/*
 *  Copyright (C) 2017  Daniel Rosales
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Additional permission under GNU GPL version 3 section 7
 *
 *  If you modify this Program, or any covered work, by linking or
 *  combining it with the uGFX Library, containing parts covered by the
 *  terms of the GFX License, Version 1.2, the licensors of this Program
 *  grant you additional permission to convey the resulting work.
 *  Corresponding Source for a non-source form of such a combination
 *  shall include the source code for the parts of the uGFX Library used
 *  as well as that of the covered work.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "fileFormat.h"
#include "fileRead.h"
#include <string.h>



int main(int argc, char** argv)
{
	Song * ttls;
	int i;
	ttls =(Song *)malloc (sizeof(ttls));
	Note * notes;
	char binname[80];
	makeBin(argv[1],binname);
	notes=readTwinkle(ttls,binname);
	printHeader(ttls);
	for(i= 0;i<ttls->sfh->numNotes;i++){
		printNote(notes);
		notes++;
	}
	freeSong(ttls);
	return 1;
}
