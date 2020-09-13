meta:
  id: thps2_pkr
  application: Tony Hawk's Pro Skater 2
  title: Tony Hawk's Pro Skater 2 PKR2 container
  file-extension: pkr
  endian: le

doc: |
  the format is known to be used in
  - Tony Hawk's Pro Skater 2
  - Magical World Racing Tour
  - Spider-Man 
  - Mat Hoffman's Pro BMX.

seq:
  - id: magic
    type: str
    size: 4
    encoding: ascii
  - id: unk1
    type: u4
  - id: num_folders
    type: u4
  - id: num_files
    type: u4
  - id: folders
    type: folder
    repeat: expr
    repeat-expr: num_folders
  - id: files
    type: file
    repeat: expr
    repeat-expr: num_files
    
instances:
  version:
    value: magic == "PKR3" ? 3 : 2 
    
types:
  folder:
    seq:
      - id: name
        type: strz
        size: 32
        encoding: ascii
      - id: offset
        type: u4
      - id: num_files
        type: u4
  file:
    seq:
      - id: name
        type: strz
        size: 32
        encoding: ascii
      - id: checksum
        type: u4
        if: _parent.version == 3
      - id: compression_type
        type: u4       
      - id: offset
        type: u4
      - id: size
        type: u4
      - id: compressed_size
        type: u4
    instances:
      data:
        pos: offset
        size: compressed_size