meta:
  id: thps3_ps2_hed
  application: Tony Hawk's Pro Skater 3 (PS2)
  file-extension: hed
  endian: le
  
seq:
  - id: entries
    type: entry
    repeat: until
    repeat-until: _.offset == -1
  
types:
  entry:
    seq:
      - id: offset
        type: s4
        
      - id: size
        type: u4
        if: offset != -1
        
      - id: name
        type: strz
        encoding: ascii
        if: offset != -1
        
      - id: padding
        size: (4 - _io.pos) % 4
        if: offset != -1