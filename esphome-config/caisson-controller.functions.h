#include <esphome.h>

std::string seconds_to_string_dhms(uint32_t seconds) 
{
  int days = 0;
  int hours = 0;
  int minuts = 0;
  if (seconds > 86399) 
  {
    days = trunc(seconds / 86400);
    seconds = seconds - (days * 86400);
  }
  if (seconds > 3599) 
  {
    hours = trunc(seconds / 3600);
    seconds = seconds - (hours * 3600);
  }
  if (seconds > 59) 
  {
    minuts = trunc(seconds / 60);
    seconds = seconds - (minuts * 60);
  }
  char buffer[14];
  sprintf(buffer, "%uд %02u:%02u:%02u", days, hours, minuts, seconds);
  return {buffer};
}

std::string seconds_to_string_hms(uint32_t seconds) 
{
  int hours = 0;
  int minuts = 0;
  if (seconds > 3599) 
  {
    hours = trunc(seconds / 3600);
    seconds = seconds - (hours * 3600);
  }
  if (seconds > 59) 
  {
    minuts = trunc(seconds / 60);
    seconds = seconds - (minuts * 60);
  }
  char buffer[10];
  sprintf(buffer, "%u:%02u:%02u", hours, minuts, seconds);
  return {buffer};
}

std::string ms_to_str(uint32_t mseconds) 
{
  int hours = 0;
  int minuts = 0;
  int seconds = 0;
  if (mseconds > 3599999) 
  {
    hours = trunc(mseconds / 3600000);
    mseconds = mseconds - (hours * 3600000);
  }
  if (mseconds > 59999) 
  {
    minuts = trunc(mseconds / 60000);
    mseconds = mseconds - (minuts * 60000);
  }
  if (mseconds > 999) 
  {
    seconds = trunc(mseconds / 1000);
    mseconds = mseconds - (seconds * 1000);
  }
  char buffer[15];
  sprintf(buffer, "%u:%02u:%02u:%03u", hours, minuts, seconds, mseconds);
  return {buffer};
}

std::string minuts_to_string_hm(uint32_t minuts) 
{
  int hours = 0;
  if (minuts > 59) 
  {
    hours = trunc(minuts / 60);
    minuts = minuts - (hours * 60);
  }
  char buffer[5];
  sprintf(buffer, "%02u:%02u", hours, minuts);
  return {buffer};
}

void clear_array_of_pump_starts()
{
  int len = sizeof(id(gs_pump_starts)) / sizeof(id(gs_pump_starts)[0]);         // Длина массива моментов времени пусков насоса
  for (int i = 0; i < len; i++) 
  {
    id(gs_pump_starts)[i] = 0;                                                  // Очистка массива
  }
}

void save_pump_start_time()
{
  int len = sizeof(id(gs_pump_starts)) / sizeof(id(gs_pump_starts)[0]);         // Длина массива моментов времени пусков насоса
  for (int i = len - 1; i > 0; i--) 
  {
    id(gs_pump_starts)[i] = id(gs_pump_starts)[i - 1];                          // Сдвижка элементов вправо
  }
  id(gs_pump_starts)[0] = millis();                                             // Сохранение момента времени текущего пуска
}

int check_count_of_pump_starts()
{
  int len = sizeof(id(gs_pump_starts)) / sizeof(id(gs_pump_starts)[0]);         // Длина массива моментов времени пусков насоса
  int count = 0;                                                                // Счётчик пусков за последний час
  uint32_t time_now = millis();                                                 // Текущий момент времени (ежеминутная проверка числа пусков выполняется от текущего момента времени)
  for (int i = 0; i < len; i++) 
  {
    if (time_now >= id(gs_pump_starts)[i])                                      // Если millis() не переполнился
    {
      if (id(gs_pump_starts)[i] != 0 && time_now - id(gs_pump_starts)[i] < 3600000) // Если это ненулевое значение и дельта менее часа
      {
        count++;
      }
    } 
    else                                                                        // Если millis() переполнился
    {
      if (id(gs_pump_starts)[i] != 0 && time_now + (sizeof(uint32_t)*0xff - id(gs_pump_starts)[i]) < 3600000) // Если это ненулевое значение и дельта менее часа
      {
        count++;
      }
    }
  }
  return {count};
}
