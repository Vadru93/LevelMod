meta:
  id: thps3_ska
  application: Tony Hawk's Pro Skater 3
  title: Tony Hawk's Pro Skater 3 skeletal animation file
  file-extension: ska
  endian: le

doc-ref: https://github.com/Vadru93/LevelMod/blob/master/formats/thps3_ska.ksy

seq:
  - id: unk1 # no effect
    type: u4
  - id: unk2 # must be negative float, usually -0.0. crashes if positive
    type: f4
  - id: duration # total anim duration, possibly in seconds
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
    doc: | 
      contains a quaternion and a time point, used to rotate bone
    seq:
    - id: x
      type: f4
    - id: y
      type: f4
    - id: z
      type: f4
    - id: w
      type: f4
    - id: time # key point in time
      type: f4
    - id: garbage # no effect, maybe stores frame number
      type: u4

  tkey:
    doc: | 
      contains a vector3 and a time point, used to move bone
    seq:
    - id: x
      type: f4
    - id: y
      type: f4
    - id: z
      type: f4
    - id: time # key point in time
      type: f4
    - id: garbage # no effect, maybe stores frame number
      type: u4