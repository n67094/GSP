
//{{BLOCK(m_engine)

//======================================================================
//
//	m_engine, 32x256@8, 
//	+ palette 256 entries, not compressed
//	+ bitmap not compressed
//	Total size: 512 + 8192 = 8704
//
//	Time-stamp: 2022-10-26, 22:35:24
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_M_ENGINE_H
#define GRIT_M_ENGINE_H

#define m_engineBitmapLen 8192
extern const unsigned int m_engineBitmap[2048];

#define m_enginePalLen 512
extern const unsigned short m_enginePal[256];

#endif // GRIT_M_ENGINE_H

//}}BLOCK(m_engine)
