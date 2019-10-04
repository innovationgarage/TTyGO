#pragma once

/*
   Copyright (C) 2018 Pharap (@Pharap)
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
        http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

// This tricks the arduboy into thinking a progmem string has been created with the `F` macro
constexpr inline const __FlashStringHelper * AsFlashStringHelper(const char * flashString)
{
  return reinterpret_cast<const __FlashStringHelper*>(flashString);
}

// This gets the length of an array at compile time
template< typename T, size_t N > constexpr size_t ArrayLength(T (&)[N])
{
  return N;
}
