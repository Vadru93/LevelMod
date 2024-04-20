meta:
  id: thps2x_bon
  application: Tony Hawk's Pro Skater 2X
  title: Tony Hawk's Pro SKater 2X (XBOX) skater mesh file
  file-extension: bon
  endian: le
doc-ref: https://github.com/Vadru93/LevelMod/blob/master/formats/thps2x_bon.ksy
doc: |
  Describes skater model in BON format found in THPS2x for the Original Xbox.
  File mapped by DCxDemo*.

seq:

  - id: magic
    contents: [Bon, 0]

  - id: version
    type: u4

  - id: num_mats
    type:
      switch-on: version
      cases:
        3: u2
        4: u4

  - id: materials
    type: material
    repeat: expr
    repeat-expr: num_mats

  - id: num_vertices
    type:
      switch-on: version
      cases:
        3: u2
        4: u4

  - id: num_unk2
    type:
      switch-on: version
      cases:
        3: u2
        4: u4

  - id: vertices # array of vetrices
    type: vertex
    repeat: expr
    repeat-expr: num_vertices

  - id: num_indices
    type:
      switch-on: version
      cases:
        3: u2
        4: u4
  - id: indices # array of tristrip indices
    type: u2
    repeat: expr
    repeat-expr: num_indices

  - id: num_hier
    type: u4

  - id: hier
    type: mesh
    repeat: expr
    repeat-expr: num_hier

types:

  material:
    seq:
      - id: name
        type: bonstring
      - id: color
        type: color
      - id: unk_float1
        type: f4
      - id: unk_float2
        type: f4
      - id: flag
        type: u1
      - id: short_name
        type: bonstring
      - id: flag1
        type: u1
      - id: flag2
        type: u1
      - id: flag3
        type: u1
      - id: size
        type: u4
      - id: data
        size: size

  mesh:
    seq:
      - id: entry_type
        type: u1
      - id: name
        type: bonstring
      - id: matrix
        type: matrix
      - id: position
        type: vector3f
      - id: num_children
        type: u2
      - id: children
        type: mesh
        repeat: expr
        repeat-expr: num_children
      - id: matrix2
        type: matrix
      - id: num_base_splits # base mesh parts
        type: u2
      - id: mat_splits
        type: mat_split
        repeat: expr
        repeat-expr: num_base_splits
      - id: num_joint_splits # stiches in the original engine
        type: u2
      - id: mat_splits2
        type: mat_split
        repeat: expr
        repeat-expr: num_joint_splits

  mat_split:
    seq:
      - id: material_index
        type: u2
      - id: offset
        type: u2
      - id: size
        type: u2

  matrix:
    seq:
      - id: entries
        type: f4
        repeat: expr
        repeat-expr: 9

  bonstring:
    seq:
      - id: length
        type: u2
      - id: content
        type: str
        encoding: ascii
        size: length

  color:
    seq:
      - id: r
        type: u1
      - id: g
        type: u1
      - id: b
        type: u1
      - id: a
        type: u1

  vector4f:
    seq:
      - id: x
        type: f4
      - id: y
        type: f4
      - id: z
        type: f4
      - id: w
        type: f4

  vector3f:
    seq:
      - id: x
        type: f4
      - id: y
        type: f4
      - id: z
        type: f4

  vector2f:
    seq:
      - id: x
        type: f4
      - id: y
        type: f4

  vertex:
    seq:
      - id: position
        type: vector4f
      - id: normal
        type: vector4f
      - id: uv
        type: vector2f