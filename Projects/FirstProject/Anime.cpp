#include "Anime.h"


Animation::Animation(qtrn rs, vec4 ps, qtrn re, vec4 pe)
	:rStart(rs), pStart(ps), rEnd(re), pEnd(pe) {}


mat4 Animation::animate(mat4 model, float delta)
{
	delta = (delta > 1)? 1 : delta;

	vec4 lrp = (1-delta)*pStart + delta*pEnd;
	mat4 lrpmat = MatrixFactory::createTranslationMatrix(lrp.x, lrp.y, lrp.z);
	qtrn sl = slerp(rStart, rEnd, delta);

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
			lastMatrix = sequence[index]->animate(model, valueK - index);
		}
	}
	return lastMatrix;
}

void Animator::update(float deltatime)
{
	valueK += (float)refwrd * deltatime / 10;
	//Verify if its needed to go to next animation in sequence
	if (valueK - index > 1) {
		index++;
		//if last animation, stop animation
		if (index >= sequence.size()) {
			index = (int) sequence.size();
			refwrd = 0;
		}
	}
	//Verify if its needed to go to next animation in sequence but while going reverse
	else if (valueK - index < 0) {
		index--;
		//if back to the end of first animation, stop animation
		if (index < 0) {
			index = 0;
			refwrd = 0;
		}
	}
}

void Animator::activate()
{
	//If animation is stopped check to see if its to go forward or reverse
	if (refwrd == 0) {
		//Forward
		if (index == 0) {
			valueK = 0;
			refwrd = 1;
		}
		//Reverse
		else {
			valueK = (float) sequence.size();
			index = (int) sequence.size() - 1;
			refwrd = -1;
		}
	}
	//if its going, start animating the opposite way
	else {
		refwrd = -refwrd;
	}
}

