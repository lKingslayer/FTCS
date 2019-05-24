"""Visualize temparature map 

"""

import os

import matplotlib 
matplotlib.use("Agg") 

import matplotlib.pyplot as plt 
import numpy as np 
from tqdm import tqdm


# config 
L, W, H, max_t = 100, 10, 5, 2
dx, dy, dz, dt = 0.1, 0.1, 0.1, 0.001 

Nx, Ny, Nz, Nt = int(L / dx + 1), int(W / dy + 1), int(H / dz + 1), int(max_t / dt + 1)


def ensure_dir(path):
    if not os.path.exists(path):
        os.mkdir(path)
    return path 


def save_heatmap(heatmap, save_path, vmin=0, vmax=100):
    
    assert len(heatmap.shape) == 2 

    print("saving image to {}".format(save_path)) 
    plt.imshow(heatmap, cmap='hot', interpolation='nearest', vmin=vmin, vmax=vmax) 

    plt.savefig(save_path) 
    plt.clf() 
    plt.close() 

def parse_txt(filename):
    # parse temparature map saved from c code

    with open(filename, 'r') as f:
        line = f.readlines()[0].strip()
        line = line.replace(" ", '')
        temp = [float(l) for l in tqdm(line.split(',')) if len(l) > 0] 

    print("reading from {}, get num temp {}.".format(filename, len(temp)))

    assert len(temp) == Nx * Ny * Nz, "len temp = {}".format(len(temp)) 
    T = np.array(temp, dtype=np.float32).reshape(Nx, Ny, Nz)
    print(T.shape)

    assert ".txt" in filename 
    t = int(filename.split("/")[-1].replace(".txt", ""))
    save_root = os.path.join(os.path.dirname(filename), "{:08d}".format(t))
    ensure_dir(save_root) 
    print('t = {}, save_root = {}'.format(t, save_root)) 
    for idx_h in range(Nz):
        save_heatmap(T[:, :, idx_h].T, 
                save_path=os.path.join(save_root, "{:04d}.png".format(idx_h)))


def parse_snapshot_file(filename):

    with open(filename, "r") as f:
        lines = f.readlines()
        lines = lines[1:]

    data = []
    for d in tqdm(lines):
        d = d.strip().replace(") = ", ",").replace("(", "") 
        d = d.split(",")
        assert len(d) == 4, "wrong length of d" 
        data.append([int(d[0]), int(d[1]), int(d[2]), float(d[3])])

    Nx = max([i[0] for i in data]) + 1
    Ny = max([i[1] for i in data]) + 1 
    Nz = max([i[2] for i in data]) + 1 
    state_t = [i[3] for i in data] 
    assert len(state_t) == Nx * Ny * Nz, "wrong length of state t" 

    state_t = np.asarray(state_t, dtype=np.float32).reshape(Nx, Ny, Nz) 
    print("shape of state t = {}".format(state_t.shape)) 

    return state_t 


def main():
    
    # check init temp map 
    # init_filename = "./snapshot/init_temp.txt"
    # parse_txt(init_filename) 

    # detecting all text 
    # text_file_root = "./snapshot"
    # text_file = os.listdir(text_file_root) 
    # text_file = [f for f in text_file if "txt" in f]

    # find the latest snapshot file 
    snapshot_files =[i for i in  os.listdir("./") if "snapshot_" in i and ".txt" in i]
    print("Get snap shot files:", snapshot_files) 
    all_times = [int(i.replace("snapshot_", "").replace(".txt", "")) for i in snapshot_files] 
    latest_time = max(all_times) 

    state_t = parse_snapshot_file("./snapshot_{}.txt".format(latest_time))

    Nz = state_t.shape[2] 
    Nx = state_t.shape[0] 
    visualization_root = "./visualize_snapshot_{}".format(latest_time) 
    ensure_dir(visualization_root) 
    for z in range(Nz):
        save_heatmap(state_t[:, :, z].T, os.path.join(visualization_root, "z_{:04d}.png".format(z))) 
    for x in range(Nx):
        save_heatmap(state_t[x, :, :], os.path.join(visualization_root, "x_{:04d}.png".format(x)))

    # for f in sorted(os.listdir(text_file_root)):
    #     parse_txt(os.path.join(text_file_root, f)) 



if __name__ == "__main__":
    main()

