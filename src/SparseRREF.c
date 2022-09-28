#include "mathlink.h"
#include "WolframLibrary.h"
#include "WolframSparseLibrary.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "spasm.h"

#include <getopt.h>
#include <math.h>
#include <err.h>
#include <sys/time.h>

#ifdef SPASM_TIMING
extern int64 reach, scatter, data_shuffling;
#endif
#include <unistd.h>

DLLEXPORT mint WolframLibrary_getVersion( ) {
	return WolframLibraryVersion;
}

DLLEXPORT int WolframLibrary_initialize( WolframLibraryData libData) {
	return 0;
}

DLLEXPORT int sparse_properties( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res) 
{
    int err = LIBRARY_NO_ERROR;
    char *what;
    mint *data;
    MSparseArray S;
    MTensor *T, Tres = 0;
    WolframSparseLibrary_Functions sparseFuns = libData->sparseLibraryFunctions;

    if (Argc != 2) return LIBRARY_FUNCTION_ERROR;

    what = MArgument_getUTF8String(Args[0]);
    S = MArgument_getMSparseArray(Args[1]);

    if (!strcmp(what, "ImplicitValue")) {
        T = (*(sparseFuns->MSparseArray_getImplicitValue))(S);
    } else if (!strcmp(what, "ExplicitValues")) {
        T = (*(sparseFuns->MSparseArray_getExplicitValues))(S);
    } else if (!strcmp(what, "RowPointers")) {
        T = (*(sparseFuns->MSparseArray_getRowPointers))(S);
    } else if (!strcmp(what, "ColumnIndices")) {
        T = (*(sparseFuns->MSparseArray_getColumnIndices))(S);
    } else if (!strcmp(what, "ExplicitPositions")) {
        err = (*(sparseFuns->MSparseArray_getExplicitPositions))(S, &Tres);
    } else if (!strcmp(what, "Normal")) {
        err = (*(sparseFuns->MSparseArray_toMTensor))(S, &Tres);
    } else {
        err = LIBRARY_FUNCTION_ERROR;
    }
    if (err) return err;
    if (!Tres) (*(libData->MTensor_clone))(*T, &Tres);

    MArgument_setMTensor(Res, Tres);
    return err;
}

DLLEXPORT int rowreduce( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res) 
{
	int Err = LIBRARY_NO_ERROR;



	MTensor Pos = 0;
	MTensor Val = 0;
	MTensor Dims = 0;

	MTensor value = 0;
	MTensor positions = 0;
	MTensor dimensionsbelow = 0;
	mint nonzerovalues;
	mint prime;
    MSparseArray A;

    mint x = 0;



	mint tmp[2];
	mint dims[2];


	


    char nnz[6];




    WolframSparseLibrary_Functions sparseFuncs = libData->sparseLibraryFunctions;

    


	if(Argc!=5) return LIBRARY_FUNCTION_ERROR; 

	Pos = MArgument_getMTensor(Args[0]);
	Val = MArgument_getMTensor(Args[1]);
	Dims = MArgument_getMTensor(Args[2]);
	nonzerovalues = MArgument_getInteger(Args[3]);
	prime = MArgument_getInteger(Args[4]);


	mint *vals;

	mint *pos;

	mint *dimensions;

		vals = libData->MTensor_getIntegerData(Val);
		
		pos = libData->MTensor_getIntegerData(Pos);

		dimensions = libData->MTensor_getIntegerData(Dims);







		int i = dimensions[0];
		int j = dimensions[1];

		/* allocate result */
		spasm_triplet *Test;
		Test = spasm_triplet_alloc(i, j, 1, prime, prime != -1);

		for(int m = 0; m< nonzerovalues; m++){
			int g = pos[2*m];
			int h = pos[2*m+1];
			int o = vals[m];
			spasm_add_entry(Test, g-1, h-1, o);
		}


		spasm_triplet_transpose(Test);
		spasm *Mtest;
		Mtest = spasm_compress(Test);
		spasm_triplet_free(Test);

		printf("1");

		spasm_lu *LU; 

		// if (!libData->AbortQ()) {
		// 	LU= spasm_LU(Mtest, SPASM_IDENTITY_PERMUTATION, 1);	
		// }	
		// else return 0;
		// int flag = 0;
		// if (!libData->AbortQ) {
		// 		LU= spasm_LU(Mtest, SPASM_IDENTITY_PERMUTATION, 1);
		// }
		// else return 0;
		LU= spasm_LU(Mtest, SPASM_IDENTITY_PERMUTATION, 1);
		spasm *U;
		U = spasm_transpose(LU->L, 1);
		spasm_make_pivots_unitary(U, SPASM_IDENTITY_PERMUTATION, U->n);
		spasm_free_LU(LU);


		int n = U->n;
		int m = U->m;
		spasm_human_format(spasm_nnz(U), nnz);
		int *p = spasm_malloc(n * sizeof(int));
		int *qinv = spasm_malloc(m * sizeof(int));
		int *Up = U->p;
		int *Uj = U->j;


		for (int j = 0; j < m; j++)
		qinv[j] = -1;
	
		for (int i = 0; i < n; i++) {		
			for (int px = Up[i]; px < Up[i + 1]; px++){
				if (qinv[Uj[px]] != -1) {
					exit(1);
				}
				if (libData->AbortQ()) {
					return 0;
				}
			}
			qinv[Uj[Up[i]]] = i;
			if (libData->AbortQ()) {
     			return 0;
   			}
		}


		spasm *R = spasm_csr_alloc(n, 100*n+m, spasm_nnz(U), U->prime, SPASM_WITH_NUMERICAL_VALUES);
		int *Rp = R->p;
		int *Rj = R->j;
		int *Rx = R->x;	
		int rnz = 0;
		int writing = 0;
		int k = 0;

		#pragma omp parallel
		{
			spasm_GFp *x = spasm_malloc(m * sizeof(*x));
			int *xj = spasm_malloc(3 * m * sizeof(int));
			spasm_vector_zero(xj, 3 * m);
			int tid = spasm_get_thread_num();
			int *qinv_local = spasm_malloc(m * sizeof(int));

			for (int j = 0; j < m; j++){
				qinv_local[j] = qinv[j];

				if (libData->AbortQ()) {
					return 0;
				}
			}
			
			// printf("1");

		
			#pragma omp for schedule(dynamic, 10)
			for (int i = 0; i < n; i++) {
				int j = Uj[Up[i]];
				assert(qinv_local[j] == i);
				qinv_local[j] = -1;
				int top = spasm_sparse_forward_solve(U, U, i, xj, x, qinv_local);

				if (libData->AbortQ()) {
     				return 0;
   				}
				
				/* ensure R has the "pivot first" property */
				for (int px = top + 1; px < m; px++)
					if (xj[px] == j) {
						xj[px] = xj[top];
						xj[top] = j;
						break;
					}
				assert(xj[top] == j);

				/* count the NZ in the new row */
				int row_nz = 0;
				for (int px = top; px < m; px++) {
					j = xj[px];
					if ((qinv_local[j] < 0) && (x[j] != 0))
						row_nz++;
				}

				/*PROBLEM HERE*/

				int row_k, row_px;
				#pragma omp critical(rref)
				{
					/* not enough room in R ? realloc twice the size */
					if (rnz + m > R->nzmax) {
						/* wait until other threads stop writing into R */
						#pragma omp flush(writing)
						while (writing > 0) {
							#pragma omp flush(writing)
						}

						int temp_check = R->nzmax;
						int update_mem = rnz - temp_check;

						spasm_csr_realloc(R, rnz + m);
						Rj = R->j;
						Rx = R->x;
					}

					// Rj = R->j;
					// Rx = R->x;


					/* save row k */
					row_k = k++;
					row_px = rnz;
					rnz += row_nz;
					/* this thread will write into R */
					#pragma omp atomic update
					writing++;
				}
				/*PROBLEM HERE*/

				/* write the new row in R */
				Rp[row_k] = row_px;
				for (int px = top; px < m; px++) {
					int j = xj[px];
					if (qinv_local[j] < 0 && x[j] != 0) {
						Rj[row_px] = xj[px];
						Rx[row_px] = x[j];
						row_px++;
					}
				}

				/* we're done writing */
				#pragma omp atomic update
				writing--;

				if (tid == 0) {
					spasm_human_format(rnz, nnz);
				}
				if (libData->AbortQ()) {
					return 0;
				}
			} /* for */
			free(x);
			free(xj);
			free(qinv_local);
		} 
		Rp[n] = rnz;
		spasm_csr_free(U);


		for (int j = 0; j < m; j++)
		qinv[j] = -1;
		for (int i = 0; i < n; i++)
		qinv[Rj[Rp[i]]] = i;
		k = 0;
		for (int j = 0; j < m; j++)
			if (qinv[j] >= 0)
				p[k++] = qinv[j];
		assert(k == n);

		spasm *S = spasm_permute(R, p, SPASM_IDENTITY_PERMUTATION, SPASM_WITH_NUMERICAL_VALUES);
		free(p);
		spasm_csr_free(R);

		m = 0;
		n = 0;
		int l = 0;
		int *Sj = S->j;
		int *Sp = S->p;
		spasm_GFp *Sx = S->x;
		n = S->n;
		m = S->m;
		int nnzs = spasm_nnz(S);
		mint mnnz = nnzs;
		MTensor Imp = 0;
		MSparseArray r = 0;


		dims[0] = nnzs;


		dims[1] = 2;
		Err = libData->MTensor_new(MType_Integer, 2, dims, &positions);
		dims[1] = 1;
		Err = libData->MTensor_new(MType_Integer, 1, dims, &value);
		

		for(l = 0; l < n; l++){
			for(int pl = Sp[l]; pl < Sp[l+1]; pl++){
			tmp[0] = pl+1;
			tmp[1] = 1;
			spasm_GFp y = (Sx != NULL) ? Sx[pl] : 1;
			// y = (y > prime / 2) ? y - prime : y;
			Err = libData->MTensor_setInteger(value, tmp, y);
			Err = libData->MTensor_setInteger(positions, tmp, l+1);
			tmp[1] = 2;
			Err = libData->MTensor_setInteger(positions, tmp, Sj[pl]+1);
			}
		}
	Err = sparseFuncs->MSparseArray_fromExplicitPositions(positions, value, Dims, Imp, &r);

	if (libData->AbortQ()){
		return 0;
	}
	if (!Err)
    MArgument_setMSparseArray(Res, r);
    
    	spasm_csr_free(S);

	// if (!Err)
	// MArgument_setInteger(Res, 1);

	if (libData->AbortQ()){
		return 0;
	}
    return Err;
}

