import json
import argparse
import os.path

parser = argparse.ArgumentParser(description='Compile JSON map file from Tiled into .map file for Regret')
parser.add_argument('--json', nargs=1, help='Path to JSON file', required=True)
parser.add_argument('--result', nargs=1, help='Path to resulting .map file', required=True)
parser.add_argument('--name', nargs=1, help='Name to use in-game for the map', required=True)
args = parser.parse_args()

#path = 'dump/testmap2.json'
#result = 'testmap2.map'
#map_name = 'Test Map 2'

# load json file
if not os.path.isfile(args.json[0]):
	print('JSON path must be a file.')
	exit(0)
f = open(args.json[0])
j = json.load(f)

# checks if all elements of a list are the same
def all_equal(x: list):
	return x.count(x[0]) == len(x)
    
# collect all layers' data
objdat = {'portals' : [], 'dialogue' : [], 'startx' : 0, 'starty' : 0}
layers = {}
xdims = []
ydims = []
for i, l in enumerate(j['layers']):
	if (l['type'] != 'tilelayer'):
		if (l['name'] == 'Object'):
			for o in l['objects']:
				props = {}
				if 'properties' in o:
					for p in o['properties']:
						props[p['name']] = p['value']
				if o['type'] == 'START':
					objdat['startx'] = o['x'] // 16
					objdat['starty'] = o['y'] // 16
				elif o['type'] == 'PORTAL':
					if 'target' not in props:
						print('Warning: Encountered invalid PORTAL object')
						continue
					portal = {'target':props['target'], 'target_x':-1,'target_y':-1,
						'x':o['x'],'y':o['y'],'w':o['width'],'h':o['height']}
					objdat['portals'].append(portal)
					if 'target_x' in props:
						portal['target_x'] = props['target_x']
					if 'target_y' in props:
						portal['target_y'] = props['target_y']
				elif o['type'] == 'DIALOGUE':
					print('Warning: DIALOGUE objects are not yet supported')
		continue
	xdims.append(l['width'])
	ydims.append(l['height'])
	layers[l['name']] = {'data' : l['data']}
    
# check that all layers have same size
if (not all_equal(xdims)) or (not all_equal(ydims)):
	print('Not all layers have matching sizes')
    
# list of required layer names
names = ['Water', 'Ground', 'Border', 'Decoration']

# check that all required layers are in map
if not all(x in list(layers.keys()) for x in names):
	print('Map does not have the required layers')
	exit(0)

# open a file to write to
r = open(args.result[0], 'w')

# write name
r.write('NAME;' + str(args.name[0]) + '\n')

# write dimensions
r.write('DIM;' + str(xdims[0]) + ',' + str(ydims[0]) + '\n')

# write each layer's data
for l in layers:
	# write name
	r.write(l.upper() + ';')
	
	# write data
	r.write(str(layers[l]['data'])[1:-1].replace(' ', '') + '\n')

# write start position
r.write('START;' + str(int(objdat['startx'])) + ',' + str(int(objdat['starty'])) + '\n')

# TODO: write portals/entities/doors
for portal in objdat['portals']:
    r.write('PORTAL;' + str(portal['target']) + ',' + str(portal['target_x']) + ',' + str(portal['target_y']))
    r.write(',' + str(int(round(portal['x']))) + ',' + str(int(round(portal['y']))) + ',')
    r.write(str(int(round(portal['w']))) + ',' + str(int(round(portal['h']))) + '\n')

# save map
r.close()

# close json
f.close()

