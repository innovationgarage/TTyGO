#ifdef DEC_SPECIAL_CHARACTER_SET
char *dec_special_character_set(unsigned char c) {
  // Map the DEC Special Graphics Character Set to UTF-8
  // Source of this mapping: http://fileformats.archiveteam.org/wiki/DEC_Special_Graphics_Character_Set
  switch (c)
  {
    case '\x5f': return "\xc2\xa0"; // NO-BREAK SPACE'
    case '\x60': return "\xe2\x97\x86"; // BLACK DIAMOND'
    case '\x61': return "\xe2\x96\x92"; // MEDIUM SHADE'
    case '\x62': return "\xe2\x90\x89"; // SYMBOL FOR HORIZONTAL TABULATION'
    case '\x63': return "\xe2\x90\x8c"; // SYMBOL FOR FORM FEED'
    case '\x64': return "\xe2\x90\x8d"; // SYMBOL FOR CARRIAGE RETURN'
    case '\x65': return "\xe2\x90\x8a"; // SYMBOL FOR LINE FEED'
    case '\x66': return "\xc2\xb0"; // DEGREE SIGN'
    case '\x67': return "\xc2\xb1"; // PLUS-MINUS SIGN'
    case '\x68': return "\xe2\x90\xa4"; // SYMBOL FOR NEWLINE'
    case '\x69': return "\xe2\x90\x8b"; // SYMBOL FOR VERTICAL TABULATION'
    case '\x6a': return "\xe2\x94\x98"; // BOX DRAWINGS LIGHT UP AND LEFT'
    case '\x6b': return "\xe2\x94\x90"; // BOX DRAWINGS LIGHT DOWN AND LEFT'
    case '\x6c': return "\xe2\x94\x8c"; // BOX DRAWINGS LIGHT DOWN AND RIGHT'
    case '\x6d': return "\xe2\x94\x94"; // BOX DRAWINGS LIGHT UP AND RIGHT'
    case '\x6e': return "\xe2\x94\xbc"; // BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL'
    case '\x6f': return "\xe2\x8e\xba"; // HORIZONTAL SCAN LINE-1'
    case '\x70': return "\xe2\x8e\xbb"; // HORIZONTAL SCAN LINE-3'
    case '\x71': return "\xe2\x94\x80"; // BOX DRAWINGS LIGHT HORIZONTAL'
    case '\x72': return "\xe2\x8e\xbc"; // HORIZONTAL SCAN LINE-7'
    case '\x73': return "\xe2\x8e\xbd"; // HORIZONTAL SCAN LINE-9'
    case '\x74': return "\xe2\x94\x9c"; // BOX DRAWINGS LIGHT VERTICAL AND RIGHT'
    case '\x75': return "\xe2\x94\xa4"; // BOX DRAWINGS LIGHT VERTICAL AND LEFT'
    case '\x76': return "\xe2\x94\xb4"; // BOX DRAWINGS LIGHT UP AND HORIZONTAL'
    case '\x77': return "\xe2\x94\xac"; // BOX DRAWINGS LIGHT DOWN AND HORIZONTAL'
    case '\x78': return "\xe2\x94\x82"; // BOX DRAWINGS LIGHT VERTICAL'
    case '\x79': return "\xe2\x89\xa4"; // LESS-THAN OR EQUAL TO'
    case '\x7a': return "\xe2\x89\xa5"; // GREATER-THAN OR EQUAL TO'
    case '\x7b': return "\xcf\x80"; // GREEK SMALL LETTER PI'
    case '\x7c': return "\xe2\x89\xa0"; // NOT EQUAL TO'
    case '\x7d': return "\xc2\xa3"; // POUND SIGN'
    case '\x7e': return "\xc2\xb7"; // MIDDLE DOT'
    default: return NULL;
  }
}
#endif
