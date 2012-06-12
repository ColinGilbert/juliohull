#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#pragma warning(disable:4996)

#include "ConvexHull.h"
#include "wavefront.h"

using namespace HACD;

int main(int argc,const char **argv)
{
	if ( argc != 2 )
	{
		printf("Usage: RaycastMesh <wavefront.obj>\r\n");
	}
	else
	{
		printf("Loading wavefront obj file '%s'\r\n", argv[1] );
		WavefrontObj obj;
		unsigned int tcount = obj.loadObj(argv[1],false);
		if ( tcount )
		{
			HullResult result;
			printf("Creating convex hull.\r\n");
			HullLibrary hl;
			HullDesc desc(obj.mVertexCount,obj.mVertices,sizeof(float)*3);
			HullError err = hl.CreateConvexHull(desc,result);
			if ( err == QE_OK )
			{
				printf("Saving convex hull as 'JulioHull.obj'\r\n");
				FILE *fph = fopen("JulioHull.obj","wb");
				if ( fph )
				{
					for (hacd::HaU32 i=0; i<result.mNumOutputVertices; i++)
					{
						const float *p = &result.mOutputVertices[i*3];
						fprintf(fph,"v %0.9f %0.9f %0.9f\r\n", p[0], p[1], p[2] );
					}
					for (hacd::HaU32 i=0; i<result.mNumTriangles; i++)
					{
						hacd::HaU32 i1 = result.mIndices[i*3+0];
						hacd::HaU32 i2 = result.mIndices[i*3+1];
						hacd::HaU32 i3 = result.mIndices[i*3+2];
						fprintf(fph,"f %d %d %d\r\n", i1+1, i2+1, i3+1 );
					}
					fclose(fph);
				}
				hl.ReleaseResult(result);
			}
			else
			{
				printf("failed to create convex hull.\r\n");
			}
		}
		else
		{
			printf("Failed to find any triangles in the input mesh.\r\n");
		}
	}
}
