
import xml.etree.ElementTree as ET
import os
import json

def main():
  compile_tilesets()
  compile_maps()

def write_json(data, name):
  filename = f'resources/maps/{name}.json'
  print(f'Writing {filename}')
  with open(filename, 'w') as f:
    f.write(json.dumps(data, indent=2))

def compile_tileset(name):

  out = {
    'name': name,
    'tiles': []
  }

  tree = ET.parse(f'tiled/{name}.tsx')
  print(f'Compiling {name}.tsx')
  root = tree.getroot()
  tiles = root.findall('tile')

  for tile in tiles:
    walkable = tile.find('properties').find('property').get('value')
    out['tiles'].append({ 'walkable': True if walkable == 'true' else False })

  return out

def compile_tilesets():
  for file in os.listdir('tiled'):
    if file.endswith('.tsx'):
      name = file[:-4]
      data = compile_tileset(name)
      write_json(data, name)

def compile_map(name):

  out = {
    'name': name,
    'tileset': None,
    'tiles': []
  }

  tree = ET.parse(f'tiled/{name}.tmx')
  print(f'Compiling {name}.tmx')
  root = tree.getroot()
  
  tiles_set = root.find('tileset')
  first_gid = int(tiles_set.get('firstgid'))
  out['tileset'] = tiles_set.get('source')[:-4]

  tile_layer = [l for l in root.findall('layer') if l.get('name') == 'tiles'][0]
  data =  tile_layer.find('data').text.strip().split(',')
  out['tiles'] = [int(tile) - first_gid for tile in data]

  return out

def compile_maps():
  for file in os.listdir('tiled'):
    if file.endswith('.tmx'):
      name = file[:-4]
      data = compile_map(name)
      write_json(data, name)

main()

  

  

  