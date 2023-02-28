#include "MatrixMultiply.hpp"

#include <exception>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <numeric>

scottgs::MatrixMultiply::MatrixMultiply() 
{
	;
}

scottgs::MatrixMultiply::~MatrixMultiply()
{
	;
}


scottgs::FloatMatrix scottgs::MatrixMultiply::operator()(const scottgs::FloatMatrix& lhs, const scottgs::FloatMatrix& rhs) const
{
    // Verify acceptable dimensions
    if (lhs.size2() != rhs.size1())
        throw std::logic_error("matrix incompatible lhs.size2() != rhs.size1()");

    scottgs::FloatMatrix result(lhs.size1(),rhs.size2());

    //define col type
    typedef boost::numeric::ublas::matrix_column<const scottgs::FloatMatrix> col_type;
    //convert boost matrix into a ptr for faster memory access
    const float * __restrict lptr = &(lhs.data()[0]);
    float * __restrict resultPtr = (float *)&(result.data()[0]);



    // YOUR ALGORIHM WITH COMMENTS GOES HERE:
    //define matrix sizes as consts
    const unsigned int l_rows = result.size1();
    const unsigned int r_cols = result.size2();
    const unsigned int d_size = lhs.size2();

    //loop over every col
    for(unsigned int j = 0; j < r_cols; ++j){
        //convert one col from matrix into a vector then get the pointer to its memory address
        col_type col (rhs, j);
        const std::vector<float> colVec(col.begin(),col.end());
        const float * __restrict cptr = &(colVec[0]);

        //loop over every row
        for(unsigned int i =0; i< l_rows; ++i){
            //get a pointer to the row from the loop
            const float * __restrict rptr = lptr + (i *d_size);
            //preform a dot product on the col form before and the row
            const float element = std::inner_product(cptr,cptr+d_size, rptr, 0.0);
            //store value into result
            resultPtr[j+i*r_cols] = element;
        }

    }




    //BLOCK ALGO ATTEMPT
//   for (unsigned int row = 0; row < l_rows; row++)
//     for (unsigned int block = 0; block < d_size; block += 2)
//       for (unsigned int chunk = 0; chunk < r_cols; chunk += 2)
//         for (unsigned int sub_chunk = 0; sub_chunk < 2; sub_chunk++)
//           for (unsigned int idx = 0; idx < 2; idx++)
//             resultPtr[row * l_rows + block + idx] +=
//                 lptr[row * d_size + chunk + sub_chunk] *
//                 rptr[chunk * r_cols + sub_chunk * l_rows + block + idx];








    //ORIGINAL ATTEMPT

    // for (unsigned int i = 0; i < l_rows; ++i) {
    //     for (unsigned int j = 0; j < r_cols; ++j) {
    //         for (unsigned int k = 0; k < d_size; ++k) {
    //             result(i,j) += lhs(i,k) * rhs(k,j);
    //         }
    //     }
    // }

    return result;
}

scottgs::FloatMatrix scottgs::MatrixMultiply::multiply(const scottgs::FloatMatrix& lhs, const scottgs::FloatMatrix& rhs) const
{
	// Verify acceptable dimensions
	if (lhs.size2() != rhs.size1())
		throw std::logic_error("matrix incompatible lhs.size2() != rhs.size1()");

	return boost::numeric::ublas::prod(lhs,rhs);
}

