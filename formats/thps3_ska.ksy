meta:
  id: thps3_ska
  application: Tony Hawk's Pro Skater 3
  title: Tony Hawk's Pro Skater 3 skeletal animation file
  file-extension: ska
  endian: le

seq:
  - id: unk1 # no effect
    type: u4
  - id: unk2 # must be negative float, usually -0.0. crashes if positive
    type: f4
  - id: unk3 # only allows small changes, affects post anim transitions
    type: f4
  - id: num_qkeys
    type: u4
  - id: num_tkeys
    type: u4
  - id: qkeys
    type: qkey
    repeat: expr
    repeat-expr: num_qkeys
  - id: tkeys
    type: tkey
    repeat: expr
    repeat-expr: num_tkeys

types:
  qkey:
    seq:
    - id: x
      type: f4
    - id: y
      type: f4
    - id: z
      type: f4
    - id: w
      type: f4
    - id: unk1
      type: f4
    - id: unk2 # garbage if unk1 is 0
      type: u4

  tkey:
    seq:
    - id: x
      type: f4
    - id: y
      type: f4
    - id: z
      type: f4
    - id: unk1
      type: f4
    - id: unk2 # garbage if unk1 is 0
      type: u4