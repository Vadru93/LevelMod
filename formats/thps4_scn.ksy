meta:
  id: thps4_scn
  application: Tony Hawk's Pro Skater 4 PC
  title: Tony Hawk's Pro Skater 4 PC Scene file
  file-extension: scn.dat
  endian: le

doc: |
  TODO:
  - update with known info
  - research flags
  - test all maps
  - research material extension
  - speedup reading, webide is super slow for some reason
  
seq:
  - id: unk1
    type: u4
  - id: unk2
    type: u4
  - id: unk3
    type: u4
  - id: num_materials
    type: u4
  - id: materials
    type: material
    repeat: expr
    repeat-expr: num_materials
  - id: num_meshes
    type: u4
  - id: meshes
    type: mesh
    repeat: expr
    repeat-expr: num_meshes
    
types:
  material:
    seq:
      - id: checkum
        type: u4
      - id: num_passes
        type: u4
      - id: unk1
        type: u4     
      - id: unk2
        type: u1
      - id: draw_order
        type: f4  
      - id: double_sided
        type: u1  
      - id: has_grass
        type: u1
      - id: grass_height
        type: f4
        if: has_grass == 1
      - id: grass_layers
        type: u4
        if: has_grass == 1
      - id: passes
        type: pass
        repeat: expr
        repeat-expr: num_passes
        
  pass:
    seq:
      - id: checksum
        type: u4
      - id: flags
        type: u4
      - id: fixed_alpha
        type: u1
      - id: f1
        type: f4
      - id: f2
        type: f4
      - id: f3
        type: f4
      - id: blend_mode
        type: u4 
      - id: u_address
        type: u4 
      - id: v_address
        type: u4 
      - id: unk4
        type: f4
      - id: byte40
        size: 40
      - id: anim
        size: 32
        if: flags & 1 > 0
      - id: extension
        type: pass_ext
        if: flags & 2 > 0
      - id: skip2
        size: 16

  pass_ext:
    seq:
      - id: num_ext
        type: u4
      - id: ext
        type: extension
        repeat: expr
        repeat-expr: num_ext

  extension:
    seq:
      - id: num_skips
        type: u4  
      - id: unk
        type: u4
      - id: skips
        size: num_skips * 8  
      
  mesh:
    seq:
      - id: checksum
        type: u4
      - id: ff
        type: u4
      - id: flag1
        type: u1
      - id: flag2
        type: u1
      - id: flags_rest
        type: u2
      - id: num_materials
        type: u4
      - id: skip40
        size: 40
      - id: num_verts
        type: u4
      - id: unk2
        type: u4
      - id: vertices
        type: vector3f
        repeat: expr
        repeat-expr: num_verts
      - id: normals
        type: vector3f
        repeat: expr
        repeat-expr: num_verts
        if: flag1 & 4 > 0
      - id: uv_passes
        type: uv_passes
        if: flag1 & 1 > 0
      - id: colors
        type: color
        repeat: expr
        repeat-expr: num_verts
        if: flag1 & 2 > 0
      - id: mat_splits
        type: mat_split
        repeat: expr
        repeat-expr: num_materials

  uv_passes:
    seq:
      - id: num_passes
        type: u4
      - id: uv_pass
        type: uv
        repeat: expr
        repeat-expr: num_passes

  uv:
    seq:
      - id: entry
        type: vector2f
        repeat: expr
        repeat-expr: _parent._parent.num_verts

  mat_split:
    seq:
      - id: zero
        type: u4
      - id: checksum
        type: u4
      - id: num_indices
        type: u4
      - id: indices
        type: u2
        repeat: expr
        repeat-expr: num_indices

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