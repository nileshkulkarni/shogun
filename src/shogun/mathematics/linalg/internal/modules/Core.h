/*
 * Copyright (c) The Shogun Machine Learning Toolbox
 * Written (w) 2014 Soumyajit De
 * Written (w) 2014 Khaled Nasr
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of the Shogun Development Team.
 */

#ifndef CORE_H_
#define CORE_H_

#include <shogun/mathematics/linalg/internal/implementation/ElementwiseSquare.h>
#include <shogun/mathematics/linalg/internal/implementation/MatrixProduct.h>
#include <shogun/mathematics/linalg/internal/implementation/Add.h>
#include <shogun/mathematics/linalg/internal/implementation/ElementwiseProduct.h>
#include <shogun/mathematics/linalg/internal/implementation/Scale.h>
#include <shogun/mathematics/linalg/internal/implementation/Convolve.h>

namespace shogun
{

namespace linalg
{

/** Performs the operation \f$C = \alpha A + \beta B\f$.
 * Works for both matrices and vectors.
 *
 * @param A First matrix
 * @param B Second matrix
 * @param C Result of the operation
 * @param alpha scaling parameter for first matrix
 * @param beta scaling parameter for second matrix
 */
template <Backend backend=linalg_traits<Core>::backend,class Matrix>
void add(Matrix A, Matrix B, Matrix C, typename Matrix::Scalar alpha=1.0,
		typename Matrix::Scalar beta=1.0)
{
	implementation::add<backend, Matrix>::compute(A, B, C, alpha, beta);
}

#ifdef HAVE_LINALG_LIB
/** Performs matrix multiplication
 *
 * @param A First matrix
 * @param B Second matrix
 * @param C Result of the operation
 * @param transpose_A Whether to the transpose of A should be used instead of A
 * @param transpose_B Whether to the transpose of B should be used instead of B
 * @param overwrite If true, the values in C are overwritten with the result,
 * otherwise, the result is added to the existing values
 */
template <Backend backend=linalg_traits<Core>::backend,class Matrix>
void matrix_product(Matrix A, Matrix B, Matrix C,
	bool transpose_A=false, bool transpose_B=false, bool overwrite=true)
{
	implementation::matrix_product<backend, Matrix>::compute(A, B, C, transpose_A, transpose_B, overwrite);
}

/** Performs the operation C = alpha*A - beta*B. Works for both matrices and vectors */
template <Backend backend=linalg_traits<Core>::backend,class Matrix>
void subtract(Matrix A, Matrix B, Matrix C,
	typename Matrix::Scalar alpha=1.0, typename Matrix::Scalar beta=1.0)
{
	implementation::add<backend, Matrix>::compute(A, B, C, alpha, -1*beta);
}

/** Performs the operation B = alpha*A. Works for both matrices and vectors */
template <Backend backend=linalg_traits<Core>::backend,class Matrix>
void scale(Matrix A, Matrix B, typename Matrix::Scalar alpha)
{
	implementation::scale<backend, Matrix>::compute(A, B, alpha);
}

/** Performs the operation C = A .* B where ".*" denotes elementwise multiplication */
template <Backend backend=linalg_traits<Core>::backend,class Matrix>
void elementwise_product(Matrix A, Matrix B, Matrix C)
{
	implementation::elementwise_product<backend, Matrix>::compute(A, B, C);
}

/**
 * Wrapper method for internal implementation of square of co-efficients that works
 * with generic dense matrices.
 *
 * @param m the matrix whose squared co-efficients matrix has to be computed
 * @return another matrix whose co-efficients are \f$m'_{i,j}=m_(i,j}^2\f$
 * for all \f$i,j\f$
 */
template <Backend backend=linalg_traits<Core>::backend,class Matrix>
typename implementation::elementwise_square<backend,Matrix>::ReturnType elementwise_square(Matrix m)
{
	return implementation::elementwise_square<backend,Matrix>::compute(m);
}

/**
 * Wrapper method for internal implementation of square of co-efficients that works
 * with generic dense matrices.
 *
 * @param m the matrix whose squared co-efficients matrix has to be computed
 * @param result Pre-allocated matrix for the result of the computation
 */
template <Backend backend=linalg_traits<Core>::backend,class Matrix, class ResultMatrix>
void elementwise_square(Matrix m, ResultMatrix result)
{
	implementation::elementwise_square<backend,Matrix>::compute(m,result);
}

/** Computes the 2D convolution of X with W
 *
 * NOTE: For the ViennaCL backend, the size of W (number of bytes) must not exceed
 * [CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE](http://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/clGetDeviceInfo.html).
 *
 * @param X Input image
 * @param W Filter coefficients. The dimensions of the matrix must be odd-numbered.
 * @param Y Output image of the same size as the input image, as the borders
 * of the input image are implicitly padded with zeros during the computation
 * @param flip If true the filter coefficients are flipped, performing cross-correlation
 * instead of convolution
 * @param overwrite If true, the values in Y are overwritten with result of the
 * computation. Otherwise, the result is added to the existing values in Y.
 * @param stride_x Stride in the x (column) direction
 * @param stride_y Stride in the y (row) direction
 */
template <Backend backend=linalg_traits<Core>::backend,class Matrix>
void convolve(Matrix X, Matrix W, Matrix Y, bool flip = false,
	bool overwrite=true, int32_t stride_x=1, int32_t stride_y=1)
{
	implementation::convolve<backend, Matrix>::compute(X, W, Y, flip, overwrite, stride_x, stride_y);
}
#endif // HAVE_LINALG_LIB

}

}
#endif // CORE_H_
