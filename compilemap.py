import json
import argparse

path = 'dump/testmap.json'
result = 'result.map'
map_name = 'Test Map'

# load json file
f = open(path)
j = json.load(f)

# checks if all elements of a list are the same
def all_equal(x: list):
	return x.count(x[0]) == len(x)
    
# collect all layers' data
layers = {}
xdims = []
ydims = []
for i, l in enumerate(j['layers']):
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
r = open(result, 'w')

# write name
r.write('NAME;' + str(map_name) + '\n')

# write dimensions
r.write('DIM;' + str(xdims[0]) + ',' + str(ydims[0]) + '\n')

# write each layer's data
for l in layers:
	# write name
	r.write(l.upper() + ';')
	
	# write data
	r.write(str(layers[l]['data'])[1:-1].replace(' ', '') + '\n')

# TODO: write start position
# TODO: write portals/entities/doors

# save map
r.close()

# close json
f.close()

