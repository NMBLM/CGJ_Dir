#include "Anime.h"


Animation::Animation(qtrn rs, vec4 ps, qtrn re, vec4 pe)
	:rStart(rs), pStart(ps), rEnd(re), pEnd(pe) {}


mat4 Animation::animate(mat4 model, float delta)
{

	vec4 lrp = (1-delta)*pStart + delta*pEnd;
	mat4 lrpmat = MatrixFactory::createTranslationMatrix(lrp.x, lrp.y, lrp.z);
	qtrn sl = slerp(rStart, rEnd, delta);
	//qtrn qv = qtrn(0, lrp.x, lrp.y, lrp.z);
	//qtrn qRot = sl * qv * inverse(sl);
	//return lrpmat * qToMatrix(qRot);
	return  lrpmat * model * qToMatrix(sl);

}


Animator::Animator()
{
}

void Animator::addAnimation(Animation * a)
{
	sequence.push_back(a);
}

mat4 Animator::calcAnimation(mat4 model)
{
	if (refwrd != 0) {
		if (index >= sequence.size()) {
			index = (int) sequence.size();
			lastMatrix = sequence[index - 1]->animate(model, 1);
		}
		else {
			lastMatrix = sequence[index]->animate(model, delta - index);
		}
	}
	return lastMatrix;
}

void Animator::update(float deltatime)
{

	if (delta - index > 1) {
		index++;
		if (index >= sequence.size()) {
			index = (int) sequence.size();
			refwrd = 0;
		}
	}
	if (delta - index < 0) {
		index--;
		if (index < 0) {
			index = 0;
			refwrd = 0;
		}
	}
	if (refwrd != 0) {
		delta += (float)refwrd * deltatime /10;
	}
}

void Animator::reset()
{
	if (refwrd == 0) {
		if (index == 0) {
			delta = 0;
			refwrd = 1;
		}
		else {
			delta = (float) index;
			refwrd = -1;
		}
	}
	else {
		refwrd = -refwrd;
	}
}

