
//{{BLOCK(capsule)

//======================================================================
//
//	capsule, 64x256@8, 
//	+ palette 256 entries, not compressed
//	+ bitmap not compressed
//	Total size: 512 + 16384 = 16896
//
//	Time-stamp: 2022-10-26, 22:35:10
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_CAPSULE_H
#define GRIT_CAPSULE_H

#define capsuleBitmapLen 16384
extern const unsigned int capsuleBitmap[4096];

#define capsulePalLen 512
extern const unsigned short capsulePal[256];

#endif // GRIT_CAPSULE_H

//}}BLOCK(capsule)
