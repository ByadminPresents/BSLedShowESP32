#include <FastLED.h>
#include <AsyncTCP.h>
#include <math.h>
#include <WiFi.h>
#include <Arduino.h>

#include <EffectsController.h>

const char *ssid = "";
const char *password = "";

const uint16_t PORT = 1234;

const uint16_t BUFFER_SIZE = 512;

#define NUM_LEDS 877

// Пин для подключения ленты
#define DATA_PIN 4 // GPIO4

CRGB leds[NUM_LEDS];

void UpdateLEDs(void *pvParameters);
void IterateOverEffects(void *pvParameters);

void extractValues(uint8_t *byte_array, uint32_t *values, size_t num_values, uint8_t mode);
void SetLightUpdateLoopState(bool state);


TaskHandle_t UpdateLEDsTask = NULL;
TaskHandle_t IterateOverEffectsTask = NULL;

EController effectsController(leds, NUM_LEDS);

bool test = false;

bool isUpdateLEDsTaskRunning = true;
bool isEffectsIterating = true;

void setup()
{
  FastLED.addLeds<WS2813, DATA_PIN, RGB>(leds, NUM_LEDS);

  xTaskCreatePinnedToCore(IterateOverEffects, "IterateOverEffects", 10000, (void*)&effectsController, 1, &IterateOverEffectsTask, 0);

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Инициализация TCP-сервера
  auto server = new AsyncServer(PORT);
  server->onClient([](void *s, AsyncClient *client)
                   {
     //Serial.println("New client connected");


    // Прием данных от клиента
    client->onData([](void* r, AsyncClient* client, void* data, size_t len) {
      //Serial.println(len);
      if (len <= BUFFER_SIZE) {
        byte bytesBuffer[len];
        memcpy(bytesBuffer, data, len);

        if (len == 0) {
          return;
        }

        size_t num_values = 0;

        uint8_t mode = bytesBuffer[0] >> 6;

        switch (mode)
        {
        case 0:
          num_values = (len * 8) / 46 * 5;
          break;
        case 1:
        case 2:
          num_values = (len * 8) / 32;
          break;
        default:
          break;
        }

        uint32_t values[num_values];

        extractValues(bytesBuffer, values, num_values, mode);

        uint32_t *effectValues = nullptr;

        if (mode == 1 && num_values > 2)
        {
          size_t byteSize = (num_values - 2) * sizeof(uint32_t);
          effectValues = (uint32_t*)malloc(byteSize);
          memcpy(effectValues, values + 2, byteSize);
        }

        switch (mode)
        {
        case 0:
          for (int i = 0; i < num_values; i += 5)
          {
            for (int j = values[i]; j <= values[i + 1] && j < NUM_LEDS; j++)
            {
              effectsController.currentLEDBuffer[j][0] = values[i + 2];
              effectsController.currentLEDBuffer[j][1] = values[i + 3];
              effectsController.currentLEDBuffer[j][2] = values[i + 4];
            }
          }
          break;
        case 1:
          effectsController.SwitchEffect(values[0], (bool)values[1], effectValues);
          break;
        case 2:
          if (values[0] == 0)
          {
              SetLightUpdateLoopState((bool)values[1]);
          }
          if (values[0] == 1)
          {
            FastLED.setBrightness(values[1]);
          }
          break;
        }
      } else {
        Serial.println("Received data exceeds buffer size!");
      }
    });

    client->onDisconnect([](void* r, AsyncClient* client) {
      delete client;
    }); },
                   NULL);

  server->begin();

  Serial.println("Server started");
}

void SetLightUpdateLoopState(bool state)
{
  if (isUpdateLEDsTaskRunning && !state)
  {
    vTaskSuspend(IterateOverEffectsTask);
    isUpdateLEDsTaskRunning = state;
  }
  else if (!isUpdateLEDsTaskRunning && state)
  {
    vTaskResume(IterateOverEffectsTask);
    isUpdateLEDsTaskRunning = state;
  }
}

void extractValues(uint8_t *byte_array, uint32_t *values, size_t num_values, uint8_t mode)
{
  size_t bit_position = 2; // Текущая позиция в битовом потоке
  size_t value_index = 0;

  switch (mode)
  {
  case 0:
  {
    while (bit_position + 46 <= 512 * 8 && value_index + 5 <= num_values)
    { // 46 битов на один набор значений
      uint64_t bit_stream = 0;
      // Собираем битовый поток из 6 байтов, начиная с текущей позиции
      for (int i = 0; i <= 6; i++)
      {
        if (i == 0)
        {
          bit_stream |= (uint64_t)(byte_array[bit_position / 8 + i] & ((1 << (8 - (bit_position % 8))) - 1)) << (40 - i * 8 + bit_position % 8);
        }
        else if (i == 6 && bit_position % 8 != 0)
        {
          bit_stream |= (uint64_t)byte_array[bit_position / 8 + i] >> (8 - bit_position % 8);
        }
        else if (i != 6)
        {
          bit_stream |= (uint64_t)byte_array[bit_position / 8 + i] << (40 - i * 8 + bit_position % 8);
        }
      }

      // Извлекаем первое значение (11 бит)
      values[value_index++] = (bit_stream >> 37) & 0x7FF;

      // Извлекаем второе значение (11 бит)
      values[value_index++] = (bit_stream >> 26) & 0x7FF;

      // Извлекаем третье значение (8 бит)
      values[value_index++] = (bit_stream >> 18) & 0xFF;

      // Извлекаем четвертое значение (8 бит)
      values[value_index++] = (bit_stream >> 10) & 0xFF;

      // Извлекаем пятое значение (8 бит)
      values[value_index++] = (bit_stream >> 2) & 0xFF;

      // Смещаем битовую позицию на 46 бит для обработки следующего набора
      bit_position += 46;
    }
  }
  case 1:
  case 2:
  {
    while (bit_position + 32 <= 512 * 8 && value_index + 1 <= num_values)
    {
      uint32_t bit_stream = 0;
      // Собираем битовый поток из 8 байтов, начиная с текущей позиции
      for (int i = 0; i <= 4; i++)
      {
        if (i == 0)
        {
          bit_stream |= (uint32_t)(byte_array[bit_position / 8 + i] & ((1 << (8 - (bit_position % 8))) - 1)) << (24 - i * 8 + bit_position % 8);
        }
        else if (i == 4 && bit_position % 8 != 0)
        {
          bit_stream |= (uint32_t)byte_array[bit_position / 8 + i] >> (8 - bit_position % 8);
        }
        else if (i != 4)
        {
          bit_stream |= (uint32_t)byte_array[bit_position / 8 + i] << (24 - i * 8 + bit_position % 8);
        }
      }

      // Извлекаем первое значение (32 бит)
      //values[value_index++] = (bit_stream >> 32) & 0xFFFFFFFF;

      // Извлекаем второе значение (32 бит)
      values[value_index++] = bit_stream;

      // Смещаем битовую позицию на 46 бит для обработки следующего набора
      bit_position += 32;
    }
  }
  }
}

void IterateOverEffects(void *pvParameters) {
  srand(esp_random());
  EController* effectsController = (EController*)pvParameters;
  for (;;)
  {
    if (isEffectsIterating)
    {
      effectsController->IterateOverEffects();
    }

    FastLED.show();
    //delay(1);
  }
  vTaskDelete(NULL);
}
int counter = 0;
void loop()
{

}