unit_vector:��λ�� vec3 ����

class ray
	origin()
	direction()
	at(double t) return orig + t*dir;

class hittable
	hit���� �ж�ray�������Ƿ��ཻ


���� ray_color(ray r) ���ݹ��߷�����ɫ
���� hit_sphere���� �жϹ��������Ƿ��ཻ
���� random_in_unit_sphere() ����һ���ڵ�λԲ�ڵ� vec3


main����
for i
	for j
	 ray r��x�� y������x��y����ray
	 ray_color��r�� ����r���õ���ɫ

