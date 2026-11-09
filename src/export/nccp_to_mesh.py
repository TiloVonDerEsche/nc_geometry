import numpy as np
import pandas as pd


def csv_to_swept_tube_obj(csv_in_path, obj_out_path, r_x=1.5, r_y=0.8, n_sides=12):
    df = pd.read_csv(csv_in_path)
    df.columns = [c.strip() for c in df.columns]

    vertices = []
    faces = []
    vertex_count = 1  # OBJ indices are 1-based

    for track_id, group in df.groupby("Track_id"):
        # Remove duplicate consecutive points that disrupt tangent calculations
        pts = group[["x", "y", "z"]].drop_duplicates().values
        if len(pts) < 2:
            continue

        # Compute track tangents along the centerline
        tangents = np.zeros_like(pts)
        tangents[:-1] = pts[1:] - pts[:-1]
        tangents[-1] = tangents[-2]

        norms = np.linalg.norm(tangents, axis=1, keepdims=True)
        norms[norms == 0] = 1.0
        tangents /= norms

        track_rings = []

        for pt, T in zip(pts, tangents):
            # Calculate orthogonal reference vectors (Normal N, Binormal B)
            up = np.array([0.0, 0.0, 1.0])
            if np.abs(np.dot(T, up)) > 0.9:
                up = np.array([0.0, 1.0, 0.0])

            N = np.cross(T, up)
            norm_N = np.linalg.norm(N)
            N = N / norm_N if norm_N > 0 else np.array([1.0, 0.0, 0.0])
            B = np.cross(T, N)

            # Generate elliptical vertex ring
            angles = np.linspace(0, 2 * np.pi, n_sides, endpoint=False)
            ring_indices = []
            for theta in angles:
                offset = r_x * np.cos(theta) * N + r_y * np.sin(theta) * B
                vertices.append(pt + offset)
                ring_indices.append(vertex_count)
                vertex_count += 1

            track_rings.append(ring_indices)

        # Connect adjacent rings into triangle quads
        for i in range(len(track_rings) - 1):
            r1, r2 = track_rings[i], track_rings[i + 1]
            for j in range(n_sides):
                j_next = (j + 1) % n_sides
                # Two triangles per quad face
                faces.append((r1[j], r2[j], r1[j_next]))
                faces.append((r1[j_next], r2[j], r2[j_next]))

    # Export OBJ file
    with open(obj_out_path, "w") as f:
        f.write("# Generated from ncc_points.csv\n")
        for v in vertices:
            f.write(f"v {v[0]:.6f} {v[1]:.6f} {v[2]:.6f}\n")
        for face in faces:
            f.write(f"f {face[0]} {face[1]} {face[2]}\n")

    print(
        f"Exported {len(vertices)} vertices and {len(faces)} faces to {obj_out_path}"
    )

if __name__ == "__main__":
    # Run conversion
    csv_to_swept_tube_obj(
    "./data/ncc_points.csv", "./data/3d_model/obj/track_mesh.obj", r_x=1.2, r_y=0.6, n_sides=12
    )
