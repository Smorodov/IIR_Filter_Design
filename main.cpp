#include "main.h"
#include "iir/Iir.h"
#include <cmath>
#include <complex>
#include <fftw3.h>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>
using namespace std;
// --------------------------------------------------------
// MATLAB функция freqz
// --------------------
std::complex<double> freqz(std::vector<double>& b, std::vector<double>& a, double f, double fs)
{
    double w = f / fs * 2 * M_PI;
    std::complex<double> num(0, 0);
    std::complex<double> den(0, 0);

    for (int i = 0; i < std::max(b.size(), a.size()); i++) {
        num += b[i % b.size()] * std::exp(std::complex<double>(0, -w * (i + 1)));
        den += a[i % a.size()] * std::exp(std::complex<double>(0, -w * (i + 1)));
    }

    auto res = num / den;
    return res;
}
// --------------------------------------------------------
// MATLAB функция freqz
// --------------------------------------------------------
std::vector<std::complex<double>> freqz_vector(std::vector<double>& b, std::vector<double>& a, int N, double fs = 1.0)
{
    std::vector<std::complex<double>> response;
    double f_step = fs * 0.5 / N;
    for (double f = 0; f < fs; f += f_step) {
        response.push_back(freqz(b, a, f, fs));
    }
    return response;
}
// --------------------------------------------------------
// Функция для расчета амплитудно-частотной характеристики
// --------------------------------------------------------
void calculateAmplitudeFrequencyResponse(
    std::vector<double>& b,
    std::vector<double>& a,
    std::vector<double>& K,
    double samplingFrequency,
    int n_points,
    std::vector<double>& freq,
    std::vector<double>& response)
{
    std::vector<std::complex<double>> h(n_points, std::complex<double>(1, 0));
    std::vector<std::complex<double>> afr;
    int ci = 0;
    for (auto k : K) {
        std::vector<double> ak = { a[ci * 3], a[ci * 3 + 1], a[ci * 3 + 2] };
        std::vector<double> bk = { k * b[ci * 3], k * b[ci * 3 + 1], k * b[ci * 3 + 2] };
        afr = freqz_vector(bk, ak, h.size(), samplingFrequency);
        // поэлементно умножаем h на afr.
        std::transform(h.begin(), h.end(), afr.begin(), h.begin(), std::multiplies<std::complex<double>>());
        ci++;
    }
    response.clear();
    freq.clear();
    double step = samplingFrequency / 2.0 / n_points;
    std::transform(h.begin(), h.end(), std::back_inserter(response), [](auto& r) { return abs(r); });
    std::generate_n(std::back_inserter(freq), h.size(), [&]() { return freq.size() * step; });
}

// --------------------------------------------------------
//
// --------------------------------------------------------
// --------------------------------------------------------
// генератор сигнала равномерного спектра
// для тестирования фмльтров.
// --------------------------------------------------------
std::vector<double> uniform_specter_ssignal(int N)
{
    std::vector<double> arr(N, 0);
    for (int n = 0; n < N; n++) {
        double t = (double)n / (double)N;
        for (int k = 0; k < N / 2; k++) {
            double omega = 2 * M_PI * k;
            arr[n] += std::cos(omega * t);
        }
    }
    return arr;
}
// --------------------------------------------------------
// ближайшая степень двойки
// --------------------------------------------------------
int pwr2(int number)
{
    int power = 0;
    while (1 << power <= number) {
        power++;
    }
    return 1 << power;
}

std::vector<double> filter(Iir::Butterworth::BandPass<4>& f, std::vector<double> x)
{
    std::vector<double> y;
    for (auto v : x) {
        y.push_back(f.filter(v));
    }
    return y;
}

// --------------------------------------------------------
// получаем спектр фурье для фмльтрованного
// тестового смгнала. Должен повторять АЧХ.
// --------------------------------------------------------
void get_fft(
    double fs,
    std::vector<double>& x,
    std::vector<double>& a,
    std::vector<double>& b,
    std::vector<double>& K,
    std::vector<double>& freq,
    std::vector<double>& fft)
{
    int n = pwr2(fs);
    double* in = fftw_alloc_real(n);
    fftw_complex* ref_out = fftw_alloc_complex(n / 2 + 1);
    fftw_complex* fftw_out = fftw_alloc_complex(n / 2 + 1);
    fftw_plan p = fftw_plan_dft_r2c_1d(n, in, fftw_out, FFTW_ESTIMATE);
    memcpy(in, x.data(), n * sizeof(double));
    fftw_execute(p);
    for (int i = 0; i < fs / 2; ++i) {
        freq.push_back(i);
        fft.push_back(((1.0 / (n / 2))) * abs(std::complex<double>(fftw_out[i][0], fftw_out[i][1])));
    }
    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(fftw_out);
}
#include "Mustache/mustache.hpp"

double wz0[4] = {0};
double wz1[4] = {0};
const double B0[4] ={ 0.362769,1,1,1 };
const double B1[4] ={ 0.725538,-2,2,-2 };
const double B2[4] ={ 0.362769,1,1,1 };
const double A0[4] ={ 1,1,1,1 };
const double A1[4] ={ 1.32516,-1.89581,1.02414,-1.77191 };
const double A2[4] ={ 0.641934,0.911044,0.286084,0.787043 };
double step(double x)
{
    double y;
    for (int i = 0; i < 4; ++i) {
        // Direct form II transposed
        y = B0[i] * x + wz0[i];
        wz0[i] = B1[i] * x - A1[i] * y + wz1[i];
        wz1[i] = B2[i] * x - A2[i] * y;
        x = y;
    }
    return y;
}

namespace mus = kainjow::mustache;
// --------------------------------------------------------
//
// --------------------------------------------------------
void main()
{
    std::vector<double> a;
    std::vector<double> b;
    std::vector<double> K;
    double FS = 1000;
    double F1 = 20;
    double F2 = 400;
    double FC = (F1 + F2) * 0.5;
    double FW = F2 - F1;
    const int N = 4;
    // Butterworth lowpass
    Iir::Butterworth::BandPass<N> f;
    f.setup(FS, FC, FW);
    // x=f.filter(x);
    Iir::Cascade::Storage cascades = f.getCascadeStorage();
    a.resize(N * 3);
    b.resize(N * 3);
    K.resize(N);
    for (int i = 0; i < N; ++i) {
        a[i * 3 + 0] = cascades.stageArray[i].getA0();
        a[i * 3 + 1] = cascades.stageArray[i].getA1();
        a[i * 3 + 2] = cascades.stageArray[i].getA2();

        b[i * 3 + 0] = cascades.stageArray[i].getB0();
        b[i * 3 + 1] = cascades.stageArray[i].getB1();
        b[i * 3 + 2] = cascades.stageArray[i].getB2();

        K[i] = 1;
    }

    std::vector<double> response;
    std::vector<double> freq;
    calculateAmplitudeFrequencyResponse(
        b,
        a,
        K,
        FS,
        256,
        freq,
        response);

    std::vector<double> x = uniform_specter_ssignal(1024);
   // x = filter(f, x);
    for(auto& e:x)
    {
        e=step(e);
    }

    std::vector<double> fft;
    std::vector<double> fft_freq;
    get_fft(FS, x, a, b, K, fft_freq, fft);

    std::vector<double> data1_x;
    std::vector<double> data1_y;
    std::vector<double> data2_x;
    std::vector<double> data2_y;

    for (int i = 0; i < response.size(); ++i) {
        if (!std::isinf(response[i])) {
            data1_x.push_back(freq[i]);
            data1_y.push_back(response[i]);
        }
    }

    for (int i = 0; i < fft.size(); ++i) {
        if (!std::isinf(fft[i])) {
            data2_x.push_back(fft_freq[i]);
            data2_y.push_back(fft[i]);
        }
    }

    draw_plots(
        data1_x,
        data1_y,
        data2_x,
        data2_y);
#include <sstream>
    std::stringstream ss;

    std::stringstream strN;
    std::stringstream strB0;
    std::stringstream strB1;
    std::stringstream strB2;
    std::stringstream strA0;
    std::stringstream strA1;
    std::stringstream strA2;
    strN << N;
    for (int i = 0; i < N; ++i) {        
        std::string s = (i == N - 1) ? "" : ",";
        strB0 << b[i * 3 + 0] << s;
        strB1 << b[i * 3 + 1] << s;
        strB2 << b[i * 3 + 2] << s;
        strA0 << a[i * 3 + 0] << s;
        strA1 << a[i * 3 + 1] << s;
        strA2 << a[i * 3 + 2] << s;
    }

    mus::mustache tmpl("double wz0[{{strN}}] = {0};\n"
 "double wz1[{{strN}}] = {0};\n"
 "const double B0[{{strN}}] ={ {{strB0}} };\n"
 "const double B1[{{strN}}] ={ {{strB1}} };\n"
 "const double B2[{{strN}}] ={ {{strB2}} };\n"
 "const double A0[{{strN}}] ={ {{strA0}} };\n"
 "const double A1[{{strN}}] ={ {{strA1}} };\n"
 "const double A2[{{strN}}] ={ {{strA2}} };\n"    
"double step(double x)\n"
"{\n"
"    double y;\n"
"    for (int i = 0; i < {{strN}}; ++i) {\n"
"        // Direct form II transposed\n"
"        y = B0[i] * x + wz0[i];\n"
"        wz0[i] = B1[i] * x - A1[i] * y + wz1[i];\n"
"        wz1[i] = B2[i] * x - A2[i] * y;\n"
"        x = y;\n"
"    }\n"
"    return y;\n"
"}\n"    
);
    mus::data var;
    var.set("strN", strN.str());
    var.set("strB0", strB0.str());
    var.set("strB1", strB1.str());
    var.set("strB2", strB2.str());
    var.set("strA0", strA0.str());
    var.set("strA1", strA1.str());
    var.set("strA2", strA2.str());
    auto res = tmpl.render(var);
    std::cout << res;
}
