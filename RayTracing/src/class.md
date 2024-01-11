unit_vector:单位化 vec3 向量

class ray
	origin()
	direction()
	at(double t) return orig + t*dir;

class hittable
	hit（） 判断ray与物体是否相交


函数 ray_color(ray r) 根据光线返回颜色
函数 hit_sphere（） 判断光线与球是否相交
函数 random_in_unit_sphere() 返回一个在单位圆内的 vec3


main函数
for i
	for j
	 ray r（x， y）根据x，y生成ray
	 ray_color（r） 根据r，得到颜色

