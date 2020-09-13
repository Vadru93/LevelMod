meta:
  id: thps_tex_dat
  file-extension: dat
  application: Tony Hawk's Pro Skater PC (4, UG, UG2)
  title: Tony Hawk's Pro Skater texture container
  endian: le

seq:
  - id: magic1
    type: u4
  - id: cnt_tex
    type: u4
  - id: textures
    type: texture
    repeat: expr
    repeat-expr: cnt_tex

types:
  texture:
    seq:
      - id: checksum
        type: u4
      - id: width
        type: u4
      - id: height
        type: u4
      - id: cnt_mip
        type: u4
      - id: bpp
        type: u4 
      - id: bpp_pal
        type: u4 
      - id: dxt_level
        type: u4 
      - id: pal_size
        type: u4
      - id: pal_data
        size: pal_size
      - id: mips
        type: mip
        repeat: expr
        repeat-expr: cnt_mip
        
  mip:
    seq:
      - id: data_size
        type: u4
      - id: data
        size: data_size