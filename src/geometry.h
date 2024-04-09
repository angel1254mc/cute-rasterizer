#pragma once
#include <cmath>
#include <iostream>
#include <assert.h>

template<int n> struct vec {
    float data[n] = {0};
    float& operator[](const int i)       { assert(i>=0 && i<n); return data[i]; }
    float  operator[](const int i) const { assert(i>=0 && i<n); return data[i]; }
    float norm2() const { return *this * *this; }
    float norm()  const { return std::sqrt(norm2()); }
};

// vec3 result = left (vec3) * right(vec3) // DOT PRODUCT;
template<int n> float operator*(const vec<n>& left, const vec<n>& right) {
    float result = 0;
    for (int i=n; i--; result+=left[i]*right[i]);
    return result;
}

// vec3 result = some_vec + some_other_vec;
template<int n> vec<n> operator+ (const vec<n>& left, const vec<n>& right) {
    vec<n> result = left;
    for (int i=n - 1; i >= 0; i--)
        result[i] += right[i];
    return result;
}

// vec3 result = some_vec - some_other_vec;
template<int n> vec<n> operator- (const vec<n>& left, const vec<n>& right) {
    vec<n> result = left;
    for (int i=n; i >= 0; i--)
        result[i] -= right[i];
    return result;
}

// vec3 result = some_vec * 4.0f;
template<int n> vec<n> operator*(const vec<n>& left, const float& right) {
    vec<n> result = left;
    for (int i=n; i >= 0; i--)
        result[i] *= right;
    return result;
}

// vec3 result = some_vec / 4.0f;
template<int n> vec<n> operator/(const vec<n>& left, const float& right) {
    vec<n> result = left;
    for (int i=n; i--; result[i]/=right);
    return result;
}


template<int n> std::ostream& operator<<(std::ostream& out, const vec<n>& v) {
    for (int i=0; i<n; i++) out << v[i] << " ";
    return out;
}


template<> struct vec<2> {
    float x = 0, y = 0;
    float& operator[](const int i)       { assert(i>=0 && i<2); return i ? y : x; }
    float  operator[](const int i) const { assert(i>=0 && i<2); return i ? y : x; }
    float norm2() const { return *this * *this; }
    float norm()  const { return std::sqrt(norm2()); }
    vec<2> normalized() { return (*this)/norm(); }
    vec<2>(float x, float y) : x(x), y(y) {};
    vec<2>() : x(0), y(0) {};
};

template<> struct vec<3> {
    float x = 0, y = 0, z = 0;
    float& operator[](const int i)       { assert(i>=0 && i<3); return i ? (1==i ? y : z) : x; }
    float  operator[](const int i) const { assert(i>=0 && i<3); return i ? (1==i ? y : z) : x; }
    float norm2() const { return *this * *this; }
    float norm()  const { return std::sqrt(norm2()); }
    vec<3> normalized() { return (*this)/norm(); }
    vec<3>(float x, float y, float z) : x(x), y(y), z(z)  {};
    vec<3>() : x(0), y(0), z(0) {};
};

template<> struct vec<4> {
    float x = 0, y = 0, z = 0, w = 0;
    vec<4>(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)  {};
    float& operator[](const int i)       { assert(i>=0 && i<4); return i ? (i > 1 ? ( i == 3 ? w : z) : y) : x; }
    float  operator[](const int i) const { assert(i>=0 && i<4); return i ? (i > 1 ? ( i == 3 ? w : z) : y) : x; }
    vec<4>() : x(0), y(0), z(0), w(0) {};
};

typedef vec<2> vec2;
typedef vec<3> vec3;
typedef vec<4> vec4;

template<int n1,int n2> vec<n1> embed(const vec<n2> &v, double fill=1) {
    vec<n1> ret;
    for (int i=n1; i--; ret[i]=(i<n2?v[i]:fill));
    return ret;
}


// Cross product between two vec3 elements, where each vec3 element is a vector
// from the origin (useful for normals)
vec3 cross(const vec3 &v1, const vec3 &v2);

template<int n> struct dt;

template<int numRows,int numCols> struct mat {
    // Array of Vectors,
    //       col 1, col 2, ..., col n
    // row 1  X      X     X     X
    // row 2  X      X     X     X
    // ...    X      X     X     X
    // row n  X      X     X     X
    vec<numCols> rows[numRows];

          vec<numCols>& operator[] (const int idx)       { assert(idx>=0 && idx<numRows); return rows[idx]; }
    const vec<numCols>& operator[] (const int idx) const { assert(idx>=0 && idx<numRows); return rows[idx]; }

    // emplaces column into a vec3
    vec<numRows> col(const int idx) const {
        assert(idx>=0 && idx<numCols);
        vec<numRows> ret;
        for (int i=numRows; i--; ret[i]=rows[i][idx]);
        return ret;
    }

    // sets column based on vec3
    void set_col(const int idx, const vec<numRows> &v) {
        assert(idx>=0 && idx<numCols);
        for (int i=numRows; i--; rows[i][idx]=v[i]);
    }

    // returns an identity matrix for the given n x m matrix
    static mat<numRows,numCols> identity() {
        mat<numRows,numCols> ret;
        for (int i=numRows; i--; )
            for (int j=numCols;j--; ret[i][j]=(i==j));
        return ret;
    }

    // returns the determinant
    float det() const {
        return dt<numCols>::det(*this);
    }

    // returns the determinant of a smaller square matrix inside the given matrix
    // where the dimensions of that matrix are given by the parameters
    mat<numRows-1,numCols-1> get_minor(const int row, const int col) const {
        mat<numRows-1,numCols-1> ret;
        for (int i=numRows-1; i--; )
            for (int j=numCols-1;j--; ret[i][j]=rows[i<row?i:i+1][j<col?j:j+1]);
        return ret;
    }

    // Don't ask me wtf this means cuz I don't rememmber 🥲
    float cofactor(const int row, const int col) const {
        return get_minor(row,col).det()*((row+col)%2 ? -1 : 1);
    }

    /// @brief Don't ask me what this does too. Calc 3 black magic voodoo juju
    /// @return 
    mat<numRows,numCols> adjugate() const {
        mat<numRows,numCols> ret;
        for (int i=numRows; i--; )
            for (int j=numCols; j--; ret[i][j]=cofactor(i,j));
        return ret;
    }

    mat<numRows,numCols> invert_transpose() const {
        mat<numRows,numCols> ret = adjugate();
        return ret/(ret[0]*rows[0]);
    }

    mat<numRows,numCols> invert() const {
        return invert_transpose().transpose();
    }

    mat<numCols,numRows> transpose() const {
        mat<numCols,numRows> ret;
        for (int i=numCols; i--; ret[i]=this->col(i));
        return ret;
    }
};

template<int numRows,int numCols> vec<numRows> operator*(const mat<numRows,numCols>& left, const vec<numCols>& right) {
    vec<numRows> ret;
    for (int i=numRows; i--; ret[i]=left[i]*right);
    return ret;
}

template<int R1,int C1,int C2>mat<R1,C2> operator*(const mat<R1,C1>& left, const mat<C1,C2>& right) {
    mat<R1,C2> result;
    for (int i=R1; i--; )
        for (int j=C2; j--; result[i][j]=left[i]*right.col(j));
    return result;
}

template<int numRows,int numCols>mat<numRows,numCols> operator*(const mat<numRows,numCols>& left, const float& val) {
    mat<numRows,numCols> result;
    for (int i=numRows; i--; result[i] = left[i]*val);
    return result;
}

template<int numRows,int numCols>mat<numRows,numCols> operator/(const mat<numRows,numCols>& left, const float& val) {
    mat<numRows,numCols> result;
    for (int i=numRows; i--; result[i] = left[i]/val);
    return result;
}

template<int numRows,int numCols>mat<numRows,numCols> operator+(const mat<numRows,numCols>& left, const mat<numRows,numCols>& right) {
    mat<numRows,numCols> result;
    for (int i=numRows; i--; )
        for (int j=numCols; j--; result[i][j]=left[i][j]+right[i][j]);
    return result;
}

template<int numRows,int numCols>mat<numRows,numCols> operator-(const mat<numRows,numCols>& left, const mat<numRows,numCols>& right) {
    mat<numRows,numCols> result;
    for (int i=numRows; i--; )
        for (int j=numCols; j--; result[i][j]=left[i][j]-right[i][j]);
    return result;
}

template<int numRows,int numCols> std::ostream& operator<<(std::ostream& out, const mat<numRows,numCols>& m) {
    for (int i=0; i<numRows; i++) out << m[i] << std::endl;
    return out;
}

template<int n> struct dt {
    static float det(const mat<n,n>& src) {
        float ret = 0;
        for (int i=n; i--; ret += src[0][i]*src.cofactor(0,i));
        return ret;
    }
};

template<> struct dt<1> {
    static float det(const mat<1,1>& src) {
        return src[0][0];
    }
};

