meta:
  id: thps2x_bon
  application: Tony Hawk's Pro Skater 2X
  title: Tony Hawk's Pro SKater 2X (XBOX) skater mesh file
  file-extension: bon
  endian: le
  
doc: |
  Describes skater model in BON format found in Original Xbox game
  Tony Hawk's Pro Skater 2X.
  TODO: skeleton in the end

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
  - id: vertices
    type: vertex
    repeat: expr
    repeat-expr: num_vertices  
  - id: num_indices
    type:
      switch-on: version
      cases:
        3: u2
        4: u4
  - id: indices
    type: u2
    repeat: expr
    repeat-expr: num_indices
    
  - id: somenum1
    type: u4
    
  - id: b1
    type: bone
  - id: b2
    type: bone
  - id: b3
    type: bone
  - id: b4
    type: bone
  - id: flts1
    type: somefloats
  - id: flts2
    type: somefloats
  - id: flts3
    type: somefloats
  - id: someshorts
    type: u2
    repeat: expr
    repeat-expr: 6
  - id: b5
    type: bone    
  - id: b6
    type: bone
  - id: b7
    type: bone
  - id: flts4
    type: somefloats
  - id: flts5
    type: somefloats
  - id: flts6
    type: somefloats
  - id: someshorts2
    type: u2
    repeat: expr
    repeat-expr: 6
    
types:

  somefloats:
    seq:
      - id: floats
        type: f4
        repeat: expr
        repeat-expr: 9
      - id: ints
        type: u2
        repeat: expr
        repeat-expr: 5
        
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
      - id: texture_size
        type: u4
      - id: texture
        size: texture_size
        
  bone:
    seq:
      - id: boneflag
        type: u1
    
      - id: name
        type: bonstring
    
      - id: matrix
        type: f4
        repeat: expr
        repeat-expr: 9
    
      - id: position
        type: vector3f
    
      - id: num_children
        type: u2