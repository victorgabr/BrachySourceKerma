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
        # extent x,y,z ( dose value at center of voxels
        self.xx = (-self.voxels[0] + 1 + 2 * x) * self.voxel_width[0] / 2
        self.yy = (-self.voxels[1] + 1 + 2 * y) * self.voxel_width[1] / 2
        self.zz = (-self.voxels[2] + 1 + 2 * z) * self.voxel_width[2] / 2


# plotting values
if __name__ == '__main__':
    # result file path
    arq = 'KermaDeposition.csv'
    # reader instance
    kerma_reader = Geant4Reader()
    kerma_reader.read(arq)

    # plotting axial, sagital and coronal slices
    # getting plot data
    dose_map = kerma_reader.matrix3D
    xx = kerma_reader.xx
    yy = kerma_reader.yy
    zz = kerma_reader.zz

    # high contrast colormap - black body radiation


    # XY plane - axial
    z_idx = int(len(zz) / 2)
    xy_plane = dose_map[:, :, z_idx] / dose_map[:, :, z_idx].max()
    fig, ax = plt.subplots()
    plt.set_cmap("nipy_spectral")
    cax = ax.imshow(xy_plane, interpolation="bicubic", extent=[xx.min(), xx.max(), yy.min(), yy.max()])
    fig.colorbar(cax)
    ax.set_xlabel("x [mm]")
    ax.set_ylabel("y [mm]")
    ax.set_title("Ir-192 - Gammamed plus z: %s mm" % str(zz[z_idx]))

    # ZX plane
    y_idx = int(len(yy) / 2)
    zx_plane = dose_map[:, y_idx, :] / dose_map[:, y_idx, :].max()
    fig, ax = plt.subplots()
    cax = ax.imshow(zx_plane, interpolation="bicubic", extent=[zz.min(), zz.max(), xx.min(), xx.max()])
    fig.colorbar(cax)
    ax.set_xlabel("z [mm]")
    ax.set_ylabel("x [mm]")
    ax.set_title("Ir-192 - Gammamed plus y: %s mm" % str(yy[y_idx]))

    # plotting values
    # zy plane
    x_idx = int(len(xx) / 2)
    zy_plane = dose_map[x_idx, :, :] / dose_map[x_idx, :, :].max()
    fig, ax = plt.subplots()
    cax = ax.imshow(zy_plane, interpolation="bicubic", extent=[zz.min(), zz.max(), yy.min(), yy.max()])
    fig.colorbar(cax)
    ax.set_xlabel("z [mm]")
    ax.set_ylabel("y [mm]")
    ax.set_title("Ir-192 - Gammamed plus x: %s mm" % str(xx[x_idx]))

    plt.show()
