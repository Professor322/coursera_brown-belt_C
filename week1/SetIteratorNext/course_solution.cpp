//
// Created by professor on 7/11/21.
//

Node* GetLeftest(Node* me) {
	assert(me);
	while (me->left) {
		me = me->left;
	}
	return me;
}


Node* Next(Node* me) {
	assert(me);

	if (me->right) {
		return GetLeftest(me->right);
	}

	if (me->parent && me->parent->left == me) {
		return me->parent;
	}

	while (me->parent && me == me->parent->right) {
		me = me->parent;
	}
	return me->parent;
}