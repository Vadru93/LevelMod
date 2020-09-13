meta:
  id: thps2x_ddm
  application: Tony Hawk's Pro Skater 2X
  title: Tony Hawk's Pro SKater 2X (XBOX) level mesh file
  file-extension: ddm
  endian: le
  
doc: |
  Describes visible level mesh in DDM format found in Original Xbox game
  Tony Hawk's Pro Skater 2X. Does not contain collision meshes. Requires
  data from a matching PSX file to fully reconstruct the level.
  File mapped by DCxDemo*.

seq:
  - id: magic
    type: u4
  - id: file_size
    type: u4
  - id: num_objects
    type: u4
  - id: entries
    type: entry
    repeat: expr
    repeat-expr: num_objects

types:

  entry:
    seq:
      - id: offset
        type: u4
      - id: length
        type: u4
    instances:
      mesh:
        type: mesh
        pos: offset
        
  mesh:
    seq:
      - id: header
        type: mesh_header
      - id: materials
        type: material
        repeat: expr
        repeat-expr: header.num_mat2
      - id: vertices
        type: vertex
        repeat: expr
        repeat-expr: header.num_vertices
      - id: indices
        type: u2
        repeat: expr
        repeat-expr: header.num_indices
      - id: mat_splits
        type: split
        repeat: expr
        repeat-expr: header.num_mat2

  mesh_header:
    seq:
      - id: index
        type: u4
      - id: checksum         
        type: u4   
      - id: anim_speed_x
        type: f4
      - id: anim_speed_y
        type: f4
      - id: unk_float
        type: f4
      - id: unk
        type: u4
      - id: flags
        type: u4
      - id: name
        type: strz
        encoding: ascii
        size: 64
      - id: unk_floats2
        type: f4
        repeat: expr
        repeat-expr: 7
      - id: num_mat
        type: u4
      - id: num_vertices
        type: u4
      - id: num_indices
        type: u4
      - id: num_mat2
        type: u4

  material:
    seq:
      - id: material_name
        type: strz
        encoding: ascii
        size: 64 
      - id: texture_name
        type: strz
        encoding: ascii
        size: 64
      - id: some_int
        type: u4
      - id: diffuse_color
        type: u4
      - id: unk0
        type: f4
      - id: unk1
        type: f4
      - id: unk2
        type: f4
      - id: unk3
        type: f4
        
  split:
    seq:
      - id: count
        type: u2
      - id: start_index
        type: u2
      - id: indices_count
        type: u2

  vertex:
    seq:
      - id: position
        type: vector3f
      - id: normal
        type: vector3f
      - id: color
        type: color
      - id: tex_coord
        type: vector2f

#basic types 
       
  vector2f:
    seq:
      - id: x
        type: f4
      - id: y
        type: f4        
        
  vector3f:
    seq:
      - id: x
        type: f4
      - id: y
        type: f4
      - id: z
        type: f4
        
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