#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/numpy.h"
#include "function1.cpp"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;


//    Passing python list to exposed funvtion

std::vector<double> list1(std::vector<double> a) {
    a[0] = 2.1;
    return a;
}
// This works as expected.
// 
//      assert test_module.list1([1,2,3]) == [2.1, 2.0, 3.0]




void list2(std::vector<double>& a) {
    a[0] = 2.1;
}
// This doesn't work as expected when bound to python. Python's list and C++ vector have different
// data representaions, so pybind11 creates it's own C++ object and pass it as reference in C++ function. It doesn't pass changes back to python.
//
//      test_list = [1.23, 3.2, 5]
//      test_module.list2(test_list)
//>     assert test_list == [2.1, 3.2, 5]
//E     assert [1.23, 3.2, 5] == [[2.1, 3.2, 5]]
//E     At index 0 diff: 1.23 != 2.1 

// The workaround for problem above is to create thin wrapper around function list2
// and then bind it as such, using return value
std::vector<double> list2_wrapper(std::vector<double> a) {
    list2(a);
    return a;
}
//     assert test_module.list2([1.23, 3.2, 5]) == [2.1, 3.2, 5]  




// Passing numpy array to exposed function


// When working with numpy arrays, passing by reference is assumed even without '&' anotation 
void np(py::array_t<double> np) {
    py::buffer_info info = np.request();
    auto ptr = static_cast<double *>(info.ptr);

    *(ptr + 1) = 34.0;
}
//    a = np.array([1.0, 2.0], dtype=np.double)
//    test_module.np(a)
//    assert np.array_equal(a, np.array([1.0, 34.0]))


// This can be used to wrap existing function in library

void existing_function(double* ptr) {
   *(ptr + 1) = 34.0; 
}

void np_wrapper(py::array_t<double> np) {
    py::buffer_info info = np.request();
    auto ptr = static_cast<double *>(info.ptr); 
 
    existing_function(ptr);
}



// The problem I'm facing here is how to wrap existing C++ function which takes `double (&arg)[]` as argument
// This is pretty much what we have in `predict_forest()` in api.cpp

void func(double (&arg)[]) {
    // 
}

void np_wrapper_2(py::array_t<double> np) {
    py::buffer_info info = arg.request();
    auto ptr = static_cast<double *>(info.ptr); 

    func(?)
}








PYBIND11_MODULE(test_module, m) {

    m.def("list1", &list1);
    m.def("list2", &list2_wrapper);
    m.def("np", &np);
    m.def("np_wrapper", &np_wrapper);

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}

