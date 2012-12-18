/*

Kernel abstract class and sample kernels

Alexander Chia
alexchia@stanford.edu
*/
#ifndef GENERICKERNELS_H_
#define GENERICKERNELS_H_

#include <vector>
#include <string>
#include <math.h>
#include <GsTLAppli/utils/named_interface.h>


// generic kernel class
//template <class T>
class Distance_kernel : public Named_interface
{
public:

	// returns name of kernel
	virtual std::string name() const = 0;
	// applies the kernel
	virtual float operator() (const float&, const float&) = 0;

	virtual Distance_kernel* clone() const=0;
};

/** sample kernels */
struct float3_t {
	float x;
	float y;
	float z;
};



class Euclidean_kernel : public Distance_kernel
{
public:

	static Named_interface* create_new_interface(std::string& ){
		return new Euclidean_kernel;
	}

	virtual std::string name() const { return "Euclidean"; }
	virtual float operator() (const float& f1, const float& f2) {
		return (f1 - f2) * (f1 - f2);
	}

	virtual Distance_kernel* clone() const {return new Euclidean_kernel;}
};

class Manhattan_kernel : public Distance_kernel
{
public:

	static Named_interface* create_new_interface(std::string& ){
		return new Manhattan_kernel;
	}

	virtual std::string name() const { return "Manhattan"; }
	virtual float operator() (const float& f1, const float& f2) {
		return fabs(f1 - f2);
	}

	virtual Distance_kernel* clone() const {return new Manhattan_kernel;}
};


class Gaussian_kernel : public Distance_kernel {
public:

	static Named_interface* create_new_interface( std::string& ){
		return new Gaussian_kernel;
	}

	Gaussian_kernel() : sigma_(1) {};
	Gaussian_kernel(float sigma) : sigma_(sigma) {};

	void setSigma(float sigma){sigma_=sigma;}
	float sigma(){return sigma_;}

	virtual std::string name() const { return "Gaussian3f"; }
	virtual float operator() (const float& f1, const float& f2) {
		return expf(-(f1-f2)*(f1-f2)/(2*sigma_*sigma_));
	}

	virtual Distance_kernel* clone() const {return new Gaussian_kernel(sigma_);}
private:
	float sigma_;
};


/*
class euclidean_kernel_vectorf : public kernel_t< std::vector<float> >
{
public:

	static Named_interface* create_new_interface( std::string& ){
		return new euclidean_kernel_vectorf;
	}

	virtual std::string name() const { return "EuclideanVectorf"; }
	virtual float operator() (const std::vector<float>& v1, const std::vector<float>& v2) {
		size_t vlen = (v1.size() < v2.size()) ? v1.size() : v2.size();
		float sum = 0.0f;
		for (unsigned int i = 0; i < vlen; i++)
			sum += (v1[i] - v2[i]) * (v1[i] - v2[i]);
		
		return sum;
	}
};

class manhattan_kernel_vectorf : public kernel_t< std::vector<float> >
{
public:

	static Named_interface* create_new_interface( std::string& ){
		return new manhattan_kernel_vectorf;
	}

	virtual std::string name() const { return "ManhattanVectorf"; }

	virtual float operator() (const std::vector<float>& v1, const std::vector<float>& v2) {
		size_t vlen = (v1.size() < v2.size()) ? v1.size() : v2.size();
		float sum = 0.0f;
		for (unsigned int i = 0; i < vlen; i++)
			sum += fabs(v1[i] - v2[i]);
		
		return sum;
	}
};

class chebyshev_kernel_vectorf : public kernel_t< std::vector<float> >
{
public:

	static Named_interface* create_new_interface( std::string& ){
		return new chebyshev_kernel_vectorf;
	}

	virtual std::string name() const { return "ChebyshevVectorf"; }
	virtual float operator() (const std::vector<float>& v1, const std::vector<float>& v2) {
		size_t vlen = (v1.size() < v2.size()) ? v1.size() : v2.size();
		float maxf = 0.0f;
		float cur;
		for (unsigned int i = 0; i < vlen; i++) {
			cur= fabs(v1[i] - v2[i]);
			if (cur > maxf) maxf = cur;
		}
		
		return maxf;
	}
};

class euclidean_kernel_3f : public kernel_t<float3_t>
{
public:

	static Named_interface* create_new_interface( std::string& ){
		return new euclidean_kernel_3f;
	}


	std::string name() const { return "Euclidean3f"; }
	float operator() (const float3_t& f1, const float3_t& f2) {
		return (f1.x - f2.x) * (f1.x - f2.x) +
						  (f1.y - f2.y) * (f1.y - f2.y) +
						  (f1.z - f2.z) * (f1.z - f2.z);
	}
};

class gaussian_kernel_3f : public kernel_t<float3_t> {
public:

	static Named_interface* create_new_interface( std::string& ){
		return new gaussian_kernel_3f;
	}

	gaussian_kernel_3f() : sigma_(1) {};

	void setSigma(float sigma){sigma_=sigma;}
	float sigma(){return sigma_;}

	virtual std::string name() const { return "Gaussian3f"; }
	virtual float operator() (const float3_t& f1, const float3_t& f2) {
		float sumsquare = (f1.x - f2.x) * (f1.x - f2.x) +
						  (f1.y - f2.y) * (f1.y - f2.y) +
						  (f1.z - f2.z) * (f1.z - f2.z);

		return expf(-sumsquare/(2*sigma_*sigma_));
	}
private:
	float sigma_;
};
*/
/*
template <class T>
Named_interface* kernel_t<T>::CreateKernel(std::string& s) {
	// create appropriate class
	if (s.compare(euclidean_kernel_1f::kernelname()) == 0)
		return new euclidean_kernel_1f();
	else if (s.compare(manhattan_kernel_1f::kernelname()) == 0)
		return new manhattan_kernel_1f();
	else if (s.compare(euclidean_kernel_vectorf::kernelname()) == 0)
		return new euclidean_kernel_vectorf();
	else
		return 0;
}
*/


#endif