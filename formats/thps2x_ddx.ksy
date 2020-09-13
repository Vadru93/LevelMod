meta:
  id: thps2x_ddx
  application: THPS2x
  title: Tony Hawk's Pro Skater 2X texture container
  file-extension: ddx
  endian: le

doc: |
  Texture container found in Original XBOX game Tony Hawk's Pro Skater 2X. 
  Represents a list of DirectX surfaces (DDS).

seq:
  - id: reserved
    type: u4
  - id: total_size
    type: u4
  - id: data_offset
    type: u4
  - id: file_count
    type: u4
  - id: files
    type: file_entry
    repeat: expr
    repeat-expr: file_count

types:
  file_entry:
    seq:
      - id: offset
        type: u4
      - id: length
        type: u4
      - id: name
        type: strz
        encoding: ascii
        size: 256
    instances:
      data:
        size: length
        pos: _root.data_offset + offset