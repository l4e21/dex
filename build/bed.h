
//{{BLOCK(bed)

//======================================================================
//
//	bed, 16x128@4, 
//	+ palette 256 entries, not compressed
//	+ 32 tiles not compressed
//	Total size: 512 + 1024 = 1536
//
//	Time-stamp: 2024-10-10, 17:03:36
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BED_H
#define GRIT_BED_H

#define bedTilesLen 1024
extern const unsigned int bedTiles[256];

#define bedPalLen 512
extern const unsigned short bedPal[256];

#endif // GRIT_BED_H

//}}BLOCK(bed)
