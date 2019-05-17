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


def save_heatmap(heatmap, save_path):
    
    assert len(heatmap.shape) == 2 

    print("saving image to {}".format(save_path)) 
    plt.imshow(heatmap, cmap='hot', interpolation='nearest') 

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


def main():
    
    # check init temp map 
    # init_filename = "./snapshot/init_temp.txt"
    # parse_txt(init_filename) 

    # detecting all text 
    text_file_root = "./snapshot"
    text_file = os.listdir(text_file_root) 
    text_file = [f for f in text_file if "txt" in f]

    for f in sorted(os.listdir(text_file_root)):
        parse_txt(os.path.join(text_file_root, f)) 



if __name__ == "__main__":
    main()

