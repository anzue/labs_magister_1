import random
import sys
from copy import deepcopy

import numpy as np
import cv2
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider
from tqdm import tqdm

sys.setrecursionlimit(1000)


def simple_kmeans(img, clusters=10):
    as_vector = img.reshape(-1, img.shape[2])
    as_vector = np.float32(as_vector)
    criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 10, 1.0)
    ret, label, center = cv2.kmeans(as_vector, clusters, None, criteria, 10, cv2.KMEANS_PP_CENTERS)
    res = center[label.flatten()]
    res = res.reshape(img.shape)
    res = res.astype(np.uint8)
    return res


def my_kmeans(img, clusters=10, iters=5):
    m, n, _ = img.shape

    centers = []
    for i in range(clusters):
        x = random.randint(0, m - 1)
        y = random.randint(0, n - 1)
        ok = True
        for x1, y1 in centers:
            if x1 == x and y1 == y:
                i -= 1
                ok = False
                break
        if ok:
            centers.append((x, y))

    print(m, n, centers)

    center_vals = np.asarray(list((img[x][y] for x, y in centers)))
    center_coords = centers

    def dist(val1, pos1, val2, pos2):
        #    return (val1 - val2).mean() + np.sqrt(np.sum(np.square(pos1 - pos2)))
        return np.abs(val1 - val2).mean() * np.log(1 + np.sqrt(np.sum(np.square(pos1 - pos2, dtype=np.float32))))

    for i in tqdm(range(iters)):
        center_vals_new = np.zeros((clusters, 3), dtype=np.uint32)
        center_coords_new = np.zeros((clusters, 2), dtype=np.uint32)
        center_counts = np.zeros((clusters, 1), dtype=int)

        for x in (range(m)):
            for y in range(n):
                best_pos = 0
                best_dist = 1e9
                for c in range(clusters):
                    cur_dist = dist(img[x, y], np.asarray((x, y)), center_vals[c], center_coords[c])
                    #  print(cur_dist)
                    if cur_dist < best_dist:
                        best_pos = c
                        best_dist = cur_dist

                center_vals_new[best_pos] += img[x][y]
                center_coords_new[best_pos] += np.asarray((x, y), dtype=np.uint32)
                center_counts[best_pos] += 1

        center_vals = center_vals_new // center_counts
        center_coords = center_coords_new // center_counts
        print(center_vals, center_coords)

    res_img = np.zeros(img.shape, dtype=np.uint8)
    for x in range(m):
        for y in range(n):
            best_pos = 0
            best_dist = 1e9
            for c in range(clusters):
                cur_dist = dist(img[x][y], np.asarray((x, y)), center_vals[c], center_coords[c])
                if cur_dist < best_dist:
                    best_pos = c
                    best_dist = cur_dist
            res_img[x][y] = center_vals[best_pos]

    return res_img


def histo_cluster(img, clusters=10, treshhold=5):
    scale = 1

    def build_histogram(img):
        histo = np.zeros((256 // scale, 256 // scale, 256 // scale), dtype=int)
        for i in range(img.shape[0]):
            for j in range(img.shape[1]):
                histo[img[i][j][0] // scale][img[i][j][1] // scale][img[i][j][2] // scale] += 1
        #  print(histo)
        histo[histo < treshhold] = 0
        # print(histo)

        return histo

    def get_color_clusters(histo):
        res = []

        def search(x, y, z):
            if x < 0 or y < 0 or z < 0 or x >= 256 // scale or y >= 256 // scale or z >= 256 // scale:
                return []
            if histo[x][y][z] == 0:
                return []
            histo[x][y][z] = 0
            res = [(x, y, z)]
            for x1 in range(-1, 2):
                for y1 in range(-1, 2):
                    for z1 in range(-1, 2):
                        if abs(x1) + abs(y1) + abs(z1) != 0:
                            res.extend(search(x + x1, y + y1, z + z1))

            return res

        for x in range(256 // scale):
            for y in range(256 // scale):
                for z in range(256 // scale):
                    if histo[x][y][z] != 0:
                        res.append(search(x, y, z))

        #print(res)
        return res

    def get_clusters_min_max(clusters):
        res = []
        for x in clusters:
            #  print(len(x), x)
            if len(x) <= 3:
                continue
            x1, y1, z1 = 256 // scale, 256 // scale, 256 // scale
            x2, y2, z2 = 0, 0, 0

            for a, b, c in x:
                x1 = min(x1, a)
                y1 = min(y1, b)
                z1 = min(z1, c)
                x2 = max(x2, a)
                y2 = max(y2, b)
                z2 = max(z2, c)
            res.append(np.array(([x1, y1, z1], [x2, y2, z2])))
      #  print(res)
        return np.asarray(res, dtype=np.int32)

    color_clusters = get_color_clusters(build_histogram(img))
 #   print(color_clusters)
    clusters_simplified = get_clusters_min_max(color_clusters)
    print(clusters_simplified)
    res_img = np.zeros(img.shape, dtype=np.uint8)
    for val in clusters_simplified:
        minv, maxv = scale * val[0], scale * val[1]
        minv = np.array(minv)
        maxv = np.array(maxv)
        print(minv, maxv)
        hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

        # lower_blue = np.array([30, 50, 50])
        # upper_blue = np.array([130, 255, 255])
        # print(lower_blue, upper_blue)

        mask = cv2.inRange(img, minv, maxv)
        result = cv2.bitwise_and(img, img, mask=mask)
        #  print(res_img.shape, result.shape)

        cur_val = (minv + maxv) // 2
        # print(np.where(mask > 0))
        print(cur_val)
        for i in range(mask.shape[0]):
            for j in range(mask.shape[1]):
                if mask[i][j]:
                    res_img[i][j] = cur_val

        # ind = np.where(mask != 0)
        # print(ind)
        # res_img[ind] = minv
        # print(res_img[ind])
        # res_img = cv2.bitwise_or(res_img, result, mask=mask)

    cv2.imshow("img", img)
    cv2.imshow("zzz", res_img)
    # cv2.waitKey(0)
    res_img = cv2.fastNlMeansDenoisingColored(res_img,None,10,10,7,21)
    return res_img


def test_basic():
    fig = plt.figure()
    ax = fig.add_subplot(111)
    fig.subplots_adjust(left=0.25, bottom=0.25)
    axmin = fig.add_axes([0.25, 0.1, 0.65, 0.03])
    smin = Slider(axmin, '2', 2, 50, valinit=2, valfmt='%0.0f')

    img = cv2.imread("data/1.jpg")
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    sz = 512
    scale_fact = min(sz / img.shape[0], sz / img.shape[1])
    w, h = int(img.shape[0] * scale_fact), int(img.shape[1] * scale_fact)
    print(w, h)
    img = cv2.resize(img, dsize=(h, w), interpolation=cv2.INTER_AREA)

    def update(val, img):
        if smin.val >= 50:
            ax.imshow(img)
            return
        #   res = my_kmeans(img, clusters=int(smin.val))
        #   res = simple_kmeans(img, clusters=int(smin.val))
        res = histo_cluster(img, treshhold=int(smin.val))
        ax.imshow(res)
        fig.canvas.draw()

    smin.on_changed(lambda x: update(x, img))
    ax.imshow(img)

    # histo_cluster(img, 10)

    plt.show()


test_basic()
