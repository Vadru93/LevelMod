meta:
  id: thps_psx
  application: Tony Hawk's Pro Skater
  title: Big Guns engine model container
  file-extension: psx
  endian: le

doc: |
  This spec is meant to fully parse PSX files found in multiple games built on 
  Neverosft's "Big guns" engine games including:
  - THPS 1,2,3,4 (release and beta versions)
  - Spiderman 1, 2
  - Apocalypse
  - Mat Hoffman's Pro BMX
    
  PSX files are containers for both models and textures. However some versions
  may split those in several files.
    
  Example:
  SKWARE.TRG - "trigger" file, used for level nodes, links to other files.
  SKWARE.PSX - level mesh
  SKWARE_L.PSX - textures
  SKWARE_O.PSX - additional items used in career mode + skybox
    
  Files tested and confirmed to work properly are:
  THPS2 PS1 (THPS): SKWARE.PSX
  THPS3 PS1 (THPS): AABURB.PSX
  
  Current spec combines the research of dcxdemo and iamgreaser:
  http://thmods.com/forum/viewtopic.php?f=5&t=200&p=1079
  https://gist.github.com/iamgreaser/b54531e41d77b69d7d13391deb0ac6a5
  
  TODO:
  parse textures
  make extensions to use enum properly
  convert coords to correct floats?
  support xbox version (6: thps2x)

seq:
  - id: version
    type: u2
    enum: version_id
  - id: magic
    contents: [2, 0]
  - id: ptr_ext
    type: u4
    
  - id: num_objects
    type: u4
  - id: objects
    type: obj
    repeat: expr
    repeat-expr: num_objects
    
  - id: num_models
    type: u4
  - id: ptr_models
    type: u4
    repeat: expr
    repeat-expr: num_models
  - id: models
    type: model
    repeat: expr
    repeat-expr: num_models
    
  - id: extensions
    type: extension
    repeat: until
    repeat-until: _.ext_type == -1
    
  - id: model_checksums
    type: u4
    repeat: expr
    repeat-expr: num_models
    
  - id: num_texture_checksums
    type: u4   
  - id: texture_checksums
    type: u4
    repeat: expr
    repeat-expr: num_texture_checksums

  - id: num_pal4
    type: u4
  - id: pal4s
    type: pal4
    repeat: expr
    repeat-expr: num_pal4
    
  - id: num_pal8
    type: u4
  - id: pal8s
    type: pal8
    repeat: expr
    repeat-expr: num_pal8    
    
  - id: num_textures
    type: u4
  - id: ptr_textures
    type: u4
    repeat: expr
    repeat-expr: num_textures        
  - id: textures
    type: texture
    repeat: expr
    repeat-expr: num_textures   
types:
  extension:
    seq:
      - id: ext_type
        type: s4
      - id: size
        type: u4
        if: ext_type != -1
      - id: data
        size: size
        if: ext_type != -1
  obj:
    seq:
      - id: u1
        type: u4
      - id: position
        type: vec4s4
      - id: u6
        type: u2
      - id: model_index
        type: u2
      - id: u71
        type: u2
      - id: u72
        type: u2
      - id: u8
        type: u4
      - id: ptr_clut
        type: u4

  model:
    seq:
      - id: unk1
        type:
          switch-on: _root.version
          cases:
            'version_id::apoc': u4
            'version_id::thps': u2
      - id: num_vertices
        type:
          switch-on: _root.version
          cases:
            'version_id::apoc': u4
            'version_id::thps': u2
      - id: num_planes
        type:
          switch-on: _root.version
          cases:
            'version_id::apoc': u4
            'version_id::thps': u2
      - id: num_faces
        type:
          switch-on: _root.version
          cases:
            'version_id::apoc': u4
            'version_id::thps': u2
      - id: radius
        type: u4
      - id: bbox
        type: bounding_box
      - id: unk
        type: u4
      - id: vertices
        type: vec4s2
        repeat: expr
        repeat-expr: num_vertices
      - id: planes
        type: vec4s2
        repeat: expr
        repeat-expr: num_planes
      - id: faces
        type: face
        repeat: expr
        repeat-expr: num_faces
        
  face:
    seq:
      - id: flags
        type: flag
      - id: size
        type: u2
      - id: indices
        type: vec4
      - id: color
        type: color32bpp  
      - id: index
        type: u2
      - id: col
        type: u2
      - id: mat_index
        type: u4
        if: flags.is_textured == true
      - id: uv
        type: uv_data
        repeat: expr
        repeat-expr: 4
        if: flags.is_textured == true
      - id: padding1
        type: u4
        if: flags.is_padded == true      

  flag:
    seq:
      - id: is_collision
        type: b1
      - id: is_transparent
        type: b1
      - id: is_padded
        type: b1
      - id: is_quad
        type: b1
      - id: f4
        type: b1
      - id: f3
        type: b1
      - id: f2
        type: b1
      - id: is_textured
        type: b1
      - id: f_rest
        type: b8
      
  uv_data:
    seq:
      - id: u
        type: u1
      - id: v
        type: u1

  vec4:
    seq:
      - id: x
        type:
          switch-on: _root.version
          cases:
            'version_id::apoc': u2
            'version_id::thps': u1
      - id: y
        type:
          switch-on: _root.version
          cases:
            'version_id::apoc': u2
            'version_id::thps': u1
      - id: z
        type:
          switch-on: _root.version
          cases:
            'version_id::apoc': u2
            'version_id::thps': u1
      - id: w
        type:
          switch-on: _root.version
          cases:
            'version_id::apoc': u2
            'version_id::thps': u1
      
  color32bpp:
    seq:
      - id: r
        type: u1
      - id: g
        type: u1
      - id: b
        type: u1
      - id: a
        type: u1

  vec4s2:
    seq:
      - id: x
        type: s2
      - id: y
        type: s2
      - id: z
        type: s2
      - id: w
        type: s2
        
  vec4s4:
    seq:
      - id: x
        type: s4
      - id: y
        type: s4
      - id: z
        type: s4
      - id: w
        type: s4
      
  bounding_box:
    seq:
      - id: xmax
        type: u2
      - id: xmin
        type: u2
      - id: ymax
        type: u2
      - id: ymin
        type: u2
      - id: zmax
        type: u2
      - id: zmin
        type: u2
        
  pal4:
    seq:
      - id: checksum
        type: u4
      - id: colors
        type: color16bpp
        repeat: expr
        repeat-expr: 16
           
  pal8:
    seq:
      - id: checksum
        type: u4
      - id: colors
        type: color16bpp
        repeat: expr
        repeat-expr: 256
        
  color16bpp:
    seq:
      - id: r
        type: b5
      - id: g
        type: b5
      - id: b
        type: b5
      - id: a
        type: b1
        
  texture:
    seq:
      - id: unk
        type: u4
      - id: pal_type
        type: u4
      - id: checksum
        type: u4
      - id: index
        type: u4
      - id: width
        type: u2
      - id: height
        type: u2
      - id: data4
        size: (width + 4 - (4 - width % 4)) * height / 2
        if: pal_type == 16
      - id: data8
        size: (width + 4 - (4 - width % 4)) * height
        if: pal_type == 256

enums:
  ext_type:
    0x06: assumed_tex_coord_anim
    0x07: assumed_vert_color_anim
    0x0A: block_map
    0x2A: unknown_2a
    0x2C: anim
    0x73424752: clut
    0x44415551: quad
    0x52454948: hier 
    -1: terminator
  
  version_id:
    3: apoc
    4: thps
    6: xbox
    
  pal_id:
    16: pal4
    256: pal8