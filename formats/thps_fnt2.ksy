meta:
  id: thps_fnt2
  application: THUG2 bitmap font
  title: Tony Hawk's Underground 2 bitmap font (FNT2)
  file-extension: fnt.xbx
  endian: le

doc: |
  Bitmap font found in Tony Hawk's Pro Skater games.
  
seq:
  - id: file_size
    type: u4
  - id: unk1
    type: u4
  - id: num_letters
    type: u4
  - id: font_height
    type: u4
  - id: font_vshift
    type: u4
  - id: letters
    type: letter
    repeat: expr
    repeat-expr: num_letters
  - id: remain_size
    type: u4
  - id: bmp
    type: bitmap
  - id: num_layouts
    type: u4
  - id: layouts
    type: rectangle
    repeat: expr
    repeat-expr: num_layouts
    
types:
  bitmap:
    seq:
    - id: width
      type: u2
    - id: height
      type: u2
    - id: bpp
      type: u2
    - id: unk1
      type: s2
    - id: unk2
      type: s2
    - id: unk3
      type: s2
    - id: data
      size: width*height
    - id: palette
      size: 256*4
      
  letter:
    seq:
      - id: vshift
        type: u2
      - id: symbol_code
        type: str
        encoding: utf-16
        size: 2
      - id: unk
        type: u2

  rectangle:
    seq:
      - id: x
        type: u2
      - id: y
        type: u2
      - id: w
        type: u2
      - id: h
        type: u2