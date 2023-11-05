// This is a sample code of video processing metrics with Prometheus.

#include <prometheus/counter.h>
#include <prometheus/exposer.h>
#include <prometheus/registry.h>

#include "config.h"
#include "opencv2/opencv.hpp"
#include "video_decoder.hpp"

int main(int argc, char** argv)
{
    // Safety check, always!
    if (argc != 2) {
        std::cout << "Usage:\n    "
                  << argv[0] << " <your-video-file>\n"
                  << std::endl;
        exit(1);
    }

    // Create directories for exporting images.
    std::filesystem::path video_file { argv[1] };
    std::cout << "Opening file: " << video_file.string() << std::endl;

    // Init the decoder
    char* filename = argv[1];
    VideoDecoder decoder { std::string(filename), AV_HWDEVICE_TYPE_CUDA };

    // Check if the decoder is valid
    std::cout << "Supported accelerator: ";
    for (auto&& acc : decoder.list_hw_accelerators())
        std::cout << acc << " " << std::endl;
    std::cout << "Valid: " << decoder.is_valid() << std::endl;
    std::cout << "Accelerated: " << decoder.is_accelerated() << std::endl;

    // Prepare memory space for frames
    uint8_t* buffer = decoder.get_buffer();
    auto [width, height] = decoder.get_frame_dims();
    std::cout << "Width: " << width << " height: " << height << std::endl;
    cv::Mat bgr(height, width, CV_8UC3, buffer, decoder.get_frame_steps());

    // Get Prometheus ready
    std::string prom_address { "127.0.0.1:8080" };
    prometheus::Exposer exposer { prom_address };
    auto registry = std::make_shared<prometheus::Registry>();
    auto& counters = prometheus::BuildCounter()
                         .Name("observed_frames_total")
                         .Help("Number of processed frames")
                         .Register(*registry);
    auto& frame_counter = counters.Add({ { "frames", "processed" } });
    exposer.RegisterCollectable(registry);
    std::cout << "Prometheus: " << prom_address + "/metrics" << std::endl;

    // Loop the video stream for frames. Press `ESC` to stop.
    int ret = 0;
    bool will_be_touched { false };
    while (ret == 0 or ret == AVERROR(EAGAIN)) {
        ret = decoder.read(will_be_touched);
        frame_counter.Increment();
#ifdef WITH_GUI
        cv::imshow("preview", bgr);
        if (cv::waitKey(1) == 27)
            break;
#endif
    }

    return 0;
}