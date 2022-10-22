读取每个点的世界坐标
$$
w4= 
\begin{bmatrix}
x\\
y\\
z\\
1
\end{bmatrix}
$$

由陀螺仪提供的四元数推算出旋转矩阵（陀螺仪坐标），结合相机坐标创建出相机外参矩阵
$$
converter=
\begin{bmatrix}
R\,\vec{T}\\
\vec{0}\;1
\end{bmatrix}
$$
相机内参矩阵已知
$$
cam_f=
\begin{bmatrix}
400. &0. &190. &0.\\
0. &400. &160. &0.\\
0. &0. &1. &0.
\end{bmatrix}
$$
根据公式可计算出像素坐标，$u3=cam_f\cdot converter\cdot w4 $ ，归一化输出即可。

$Remark$：对外参矩阵的$\vec{T}$还不是很理解。代码参考了pdf和@王宇轩学长的代码。