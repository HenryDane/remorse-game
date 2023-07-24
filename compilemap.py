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
objdat = {'portals' : [], 'dialogue' : [], 'chests' : [], 'startx' : 0, 'starty' : 0,
          'doors' : [], 'switches' : [], 'traps' : []}
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
                    if o['name']:
                        portal['name'] = o['name']
                    else:
                        portal['name'] = '_'
                    if 'target_x' in props:
                        portal['target_x'] = props['target_x']
                    if 'target_y' in props:
                        portal['target_y'] = props['target_y']
                    objdat['portals'].append(portal)
                elif o['type'] == 'DIALOGUE':
                    print('Warning: DIALOGUE objects are not yet supported')
                elif o['type'] == 'CHEST':
                    chest = {'x': o['x'] // 16, 'y' : o['y'] // 16, 'type' : o['gid'] - 1, 'loot': []}
                    for p in props:
                        chest['loot'].append({'item':p,'value':props[p]})
                    if o['name']:
                        chest['name'] = o['name']
                    else:
                        chest['name'] = '_'
                    objdat['chests'].append(chest)
                elif o['type'] == 'DOOR':
                    door = {'x' : o['x'] // 16, 'y' : o['y'] // 16, 'type' : o['gid'] - 1, 'loot': []}
                    if 'alt' not in props:
                        print('Warning: Found door with no ALT specified.')
                        continue
                    door['alt'] = props['alt']
                    if o['name']:
                        door['name'] = o['name']
                    else:
                        door['name'] = '_'
                    if 'trigger_type' not in props:
                        #print('Warning: Found door with no trigger type.')
                        door['trigger_type'] = '__invalid__'
                    else:
                        door['trigger_type'] = props['trigger_type']
                    if 'toggleable' not in props:
                        #print('Warning: Found door with no toggleable property.')
                        door['toggleable'] = 0
                    else:
                        door['toggleable'] = props['toggleable']
                    objdat['doors'].append(door)
                elif o['type'] == 'SWITCH':
                    switch = {'x' : o['x'] // 16, 'y' : o['y'] // 16, 'type' : o['gid'] - 1, 'loot': []}
                    if 'alt' not in props:
                        print('Warning: Found switch with no ALT specified.')
                        continue
                    switch['alt'] = props['alt']
                    switch['require_previous'] = props.get('require_previous', '_')
                    switch['reset_counter'] = props.get('reset_counter', -1)
                    if o['name']:
                        switch['name'] = o['name']
                    else:
                        switch['name'] = '_'
                    objdat['switches'].append(switch)
                elif o['type'] == 'TRAP':
                    trap = {'x' : o['x'] // 16, 'y' : o['y'] // 16, 'type' : o['gid'] - 1, 'loot': []}
                    if 'alt' not in props:
                        print('Warning: Found trap with no ALT specified.')
                        continue
                    trap['alt'] = props['alt']
                    trap['damage'] = props.get('damage', 0)
                    trap['dx'] = props.get('dx', 0)
                    trap['dy'] = props.get('dy', 0)
                    trap['trigger_arm'] = props.get('trigger_arm', '_')
                    trap['trigger_disarm'] = props.get('trigger_disarm', '_')
                    if o['name']:
                        trap['name'] = o['name']
                    else:
                        trap['name'] = '_'
                    objdat['traps'].append(trap)
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

# write portals/entities/doors
for portal in objdat['portals']:
    r.write('PORTAL;')
    if 'name' in portal:
        r.write(portal['name'] + ';')
    r.write(str(portal['target']) + ',' + str(portal['target_x']) + ',' + str(portal['target_y']))
    r.write(',' + str(int(round(portal['x']))) + ',' + str(int(round(portal['y']))) + ',')
    r.write(str(int(round(portal['w']))) + ',' + str(int(round(portal['h']))) + '\n')

# write chests
# CHEST;10,10,742,Golden Armor:10:100%|
for chest in objdat['chests']:
    if 'name' in chest:
        r.write('CHEST;' + str(chest['name']) + ';' + str(chest['x']) + ',' + str(chest['y']) + 
            ',' + str(chest['type']) + ',')    
    else:    
        r.write('CHEST;' + str(chest['x']) + ',' + str(chest['y']) + ',' + str(chest['type']) + ',')
    for i, l in enumerate(chest['loot']):
        r.write(l['item'] + ':' + str(l['value']))
        if (i != len(chest['loot']) - 1):
            r.write('|')
    r.write('\n')
    
# write doors (x,y,gid,alt,trigger type, toggleable)
# DOOR;1,2,50,51,KEY:Blue Key,0
for door in objdat['doors']:
    r.write('DOOR;')
    if 'name' in door:
        r.write(door['name'] + ';')
    r.write(str(door['x']) + ',' + str(door['y']) + ',' + str(door['type']) + 
        ',' + str(door['alt']) + ',' + str(door['trigger_type']) + ',' + str(int(door['toggleable'])))
    r.write('\n')
    
# write switches (x, y, gid, alt, rq_prev, rs_cnt)
for switch in objdat['switches']:
    r.write('SWITCH;')
    if 'name' in switch:
        r.write(switch['name'] + ';')
    r.write(str(switch['x']) + ',' + str(switch['y']) + ',' + str(switch['type']) + 
        ',' + str(switch['alt']) + ',' + str(switch['require_previous']) + ',' +
        str(switch['reset_counter']))
    r.write('\n')
    
for trap in objdat['traps']:
    r.write('TRAP;')
    if 'name' in trap:
        r.write(trap['name'] + ';')
    r.write(str(trap['x']) + ',' + str(trap['y']) + ',' + str(trap['type']) + 
        ',' + str(trap['alt']) + ',' + str(trap['damage']) + ',' + str(trap['dx']) +
        ',' + str(trap['dy']) + ',' + str(trap['trigger_arm']) + ',' + str(trap['trigger_disarm']))
    r.write('\n')
    
# save map
r.close()

# close json
f.close()

