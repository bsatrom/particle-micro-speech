/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "Particle.h"
#include <TensorFlowLite.h>
#include "tensorflow/lite/experimental/micro/examples/micro_speech/command_responder.h"

extern int tflite_micro_main(int argc, char *argv[]);
extern unsigned char model_data[];
extern const char *model_labels[];

// Init the model as a global to ensure it goes onto flash and stays off the heap
const unsigned char *g_tiny_conv_micro_features_model_data = (unsigned char *)model_data;
extern unsigned char kCategoryCount;
extern const char *model_labels[];
char **kCategoryLabels;

void setup()
{
  pinMode(D7, OUTPUT);

  Serial.begin();

  Mesh.off();
  BLE.off();

  Serial.printf("Model Data: ", g_tiny_conv_micro_features_model_data, model_data);
  for (uint8_t x = 0; x < 16; x++)
  {
    Serial.printf("0x%02X, ", g_tiny_conv_micro_features_model_data[x]);
  }
  Serial.println("...");

  kCategoryLabels = (char **)malloc(kCategoryCount * sizeof(char **));
  Serial.printf("Found %d labels:\n", kCategoryCount);
  for (int i = 0; i < kCategoryCount; i++)
  {
    kCategoryLabels[i] = (char *)malloc(strlen(model_labels[i]) + 1);
    strcpy(kCategoryLabels[i], model_labels[i]);
    Serial.print("\t");
    Serial.println(kCategoryLabels[i]);
  }

  Serial.println("-----------TFLITE----------");

  tflite_micro_main(0, NULL);
}

void loop()
{
}

void RespondToCommand(tflite::ErrorReporter *error_reporter,
                      int32_t current_time, const char *found_command,
                      uint8_t score, bool is_new_command)
{
  Serial.println("In RespondToCommand");

  if (is_new_command)
  {
    error_reporter->Report("I heard %s (score %d) @%dms (realtime %d)", found_command, score,
                           current_time, millis());
  }
}
