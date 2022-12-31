#include "union_find.hpp"
#include <iostream>
using namespace std;

UnionFind::UnionFind() {
	for(int i = 0; i < UNION_FIND_SIZE; i++) sets[i] = NULL;
	number = 0;
}
void UnionFind::destroy() {
	for(int i = 0; i < UNION_FIND_SIZE; i++) delete sets[i];
}


void UnionFind::makeSet(s_coord s) {
	if(sets[s] == NULL) {
		sets[s] = new UFelement;
		sets[s]->val = s;
		sets[s]->weight = 1;
		sets[s]->parent = sets[s];
		sets[s]->next = sets[s];
		sets[s]->last = sets[s];
		number++;
	}
}
void UnionFind::print(char m[ROOM_AREA]) {
	char map[ROOM_HEIGHT_T][ROOM_WIDTH_T]; 

	for(int y = 0; y < ROOM_HEIGHT_T; y++) {
		for(int x = 0; x< ROOM_WIDTH_T; x++) {
			map[y][x] = m[Coordinate(x,y).single_set(Coordinate(ROOM_WIDTH,ROOM_HEIGHT))];
		}
	}
	char ch = 'A';
	bool used[ROOM_AREA];
	for(int i = 0; i < ROOM_AREA; i++) used[i] = false;
	for(int i = 0; i < ROOM_AREA; i++) {
		if(!used[i] && sets[i] != NULL) {
			pUFelement p = sets[i];
			do {
				used[p->val] = true;
				Coordinate c = Coordinate(p->val, Coordinate(ROOM_WIDTH, ROOM_HEIGHT));
				map[c.inty()][c.intx()] = ch;
				p = p->next;
			} while(p != sets[i]);
			if(ch == 'Z') ch = 'a';
			else ch++;
		}
	}

	for(int y = 0; y < ROOM_HEIGHT_T; y++) {
		for(int x = 0;x < ROOM_WIDTH_T; x++) {
			cout<<map[y][x];
		}cout<<endl;
	}cout<<"\n\n";
}
void UnionFind::print(char m[20][70]) {
	char map[ROOM_HEIGHT_T][ROOM_WIDTH_T]; 

	for(int y = 0; y < ROOM_HEIGHT_T; y++) {
		for(int x = 0; x< ROOM_WIDTH_T; x++) {
			map[y][x] = m[y][x];
		}
	}
	char ch = 'A';
	bool used[ROOM_AREA];
	for(int i = 0; i < ROOM_AREA; i++) used[i] = false;
	for(int i = 0; i < ROOM_AREA; i++) {
		if(!used[i] && sets[i] != NULL) {
			pUFelement p = sets[i];
			do {
				used[p->val] = true;
				Coordinate c = Coordinate(p->val, Coordinate(ROOM_WIDTH, ROOM_HEIGHT));
				map[c.inty()][c.intx()] = ch;
				p = p->next;
			} while(p != sets[i]);
			if(ch == 'Z') ch = 'a';
			else ch++;
		}
	}

	for(int y = 0; y < ROOM_HEIGHT_T; y++) {
		for(int x = 0;x < ROOM_WIDTH_T; x++) {
			cout<<map[y][x];
		}cout<<endl;
	}cout<<"\n\n";
}

s_coord UnionFind::find(s_coord s) {
	if(sets[s] == NULL) return ERROR_INT;
	else return sets[s]->parent->val;
}
void UnionFind::merge(s_coord a, s_coord b) {
	if(sets[a] != NULL && sets[b] != NULL && find(a) != find(b)) {
		pUFelement minor, major;
		if(sets[a]->parent->weight > sets[a]->parent->weight) {
			minor = sets[b]->parent;
			major = sets[a]->parent;
		} else {
			minor = sets[a]->parent;
			major = sets[b]->parent;
		}
		//aggiorna puntatori a parent
		pUFelement p = minor;
		int num = 0;
		do {
			p->parent = major;
			p = p->next;
			num++;
		} while(p != minor);
		major->weight += num;
		//unisci liste circolari aggiornando next e last
		pUFelement tmp = major->last;
		tmp->next = minor;
		minor->last->next = major;
		major->last = minor->last;
		minor->last = tmp;

		number--;
	}
}


//// GET
s_coord UnionFind::next(s_coord set) {
	if(sets[set] == NULL) return ERROR_INT;
	else return sets[set]->next->val;
}
int UnionFind::getNumber() {
	return number;
}
s_coord UnionFind::firstSet() {
	if(number == 0) return -1;
	else {
		int i = 0;
		while(sets[i] == NULL) i++;
		return sets[i]->parent->val;
	}
}