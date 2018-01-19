import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

class Geant4Reader:
    def __init__(self):
        self.size = np.zeros(3)
        self.voxels = np.zeros(3)
        self.voxel_width = np.zeros(3)
        self.xx = np.zeros(1)
        self.yy = np.zeros(1)
        self.zz = np.zeros(1)
        self.matrix3D = []

    def read(self, filename):
        self.read_header(filename)
        self.read_matrix(filename)
        self.calc_coordinates()

    def read_header(self, filename):
        # GETTING scoring sizes
        with open(filename) as f:
            txt = f.readlines()
            self.size = np.asarray(eval(txt[2].split(":")[-1]))
            self.voxels = np.asarray(eval(txt[3].split(":")[-1]))
            # TODO add unit specification
        # TODO add unit tests
        # GEANT4 specifies volumes in half sizes
        self.voxel_width = self.size / self.voxels * 2

    def read_matrix(self, filename):
        # reading output file
        df = pd.read_csv(filename, skiprows=5, header=None)
        vals = df.values
        tmp = np.zeros((self.voxels[0], self.voxels[1], self.voxels[2]))
        for row in vals:
            r, c, d = row[:3].astype(int)
            tmp[r, c, d] = row[3]

        self.matrix3D = tmp

        return tmp

    def calc_coordinates(self):
        # calculating coordinates
        x = np.arange(self.voxels[0])
        y = np.arange(self.voxels[1])
        z = np.arange(self.voxels[2])
        # extent x,y,z
        self.xx = (-self.voxels[0] + 1 + 2 * x) * self.voxel_width[0] / 2
        self.yy = (-self.voxels[1] + 1 + 2 * y) * self.voxel_width[1] / 2
        self.zz = (-self.voxels[2] + 1 + 2 * z) * self.voxel_width[2] / 2


# plotting values
if __name__ == '__main__':
    arq = 'KermaDeposition.csv'
    # reader instance
    kerma_reader = Geant4Reader()
    kerma_reader.read(arq)

    dose_map = kerma_reader.matrix3D
    xx = kerma_reader.xx
    yy = kerma_reader.yy
    zz = kerma_reader.zz

    plt.set_cmap("nipy_spectral")
    # XY plane - axial
    z_idx = 0
    im = dose_map[:, :, z_idx]
    vmin =im.min()
    vmax = im.max()
    im /= vmax
    im *= 100

    plt.imshow(im, interpolation="bicubic",
               extent=[xx.min(), xx.max(), yy.min(), yy.max()],
               vmin=0,
               vmax=100)
    plt.colorbar()
    plt.xlabel("x [mm]")
    plt.ylabel("y [mm]")
    plt.title(" Ir-192 source - TG186 - relative rose - position z: %s mm" % str(zz[z_idx]))
    plt.show()