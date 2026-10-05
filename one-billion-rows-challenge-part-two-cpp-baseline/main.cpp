#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;
using namespace chrono;

struct SDetail
{
    int64_t count = 0, min = INT64_MAX, max = INT64_MIN, sum = 0;
};

static void ReadWholeFile(const string& filePath, vector<char>& buffer)
{
    const auto size = filesystem::file_size(filePath);
    buffer.resize(size);
    ifstream file(filePath, ios::in | ios::binary);
    uintmax_t position = 0;
    while (position < size)
    {
        file.read(buffer.data() + position, min(4096ull, size - position));
        position += file.gcount();
    }
}

static optional<tuple<string_view, string_view>> ParseNextLine(const vector<char>& buffer, int64_t& position)
{
    if (position < 0)
        throw runtime_error("Position can't be negative");
    if (buffer.size() <= position)
        return {};
    if (buffer[position] == '\n')
        return {};

    const auto scPos = find(buffer.begin() + position, buffer.end(), ';');
    if (scPos == buffer.end())
        throw runtime_error("Semicolon position not found");
    auto name = string_view(buffer.data() + position, (scPos - buffer.begin()) - position);
    position = (scPos - buffer.begin()) + 1;

    const auto tempPos = find(buffer.begin() + position, buffer.end(), '\n');
    if (tempPos == buffer.end())
        throw runtime_error("Temperature position not found");
    auto temp = string_view(buffer.data() + position, (tempPos - buffer.begin()) - position);
    position = (tempPos - buffer.begin()) + 1;

    return tuple{name, temp};
}

static int64_t ParseTemperature(const string_view temperature)
{
    char* dummy = nullptr;
    return static_cast<int64_t>(std::strtod(temperature.data(), &dummy) * 10);
}

int main()
{
    vector<char> buffer;
    ReadWholeFile(R"(C:\OBC.csv)", buffer);
    int64_t position = 0;
    map<string_view, SDetail> resultMap;
    while (const auto lineOpt = ParseNextLine(buffer, position))
    {
        auto line = lineOpt.value();
        const auto temp = ParseTemperature(get<1>(line));
        auto& detail = resultMap[get<0>(line)];
        detail.count++;
        detail.min = min(detail.min, temp);
        detail.max = max(detail.max, temp);
        detail.sum += temp;
    }
    string result = "{";
    for (auto& [station, detail] : resultMap)
    {
        result.append(format("{}={:.1f}/{:.1f}/{:.1f},", station, static_cast<double>(detail.min) / 10.0,
                             (static_cast<double>(detail.sum) / static_cast<double>(detail.count)) / 10.0, static_cast<double>(detail.max) / 10.0));
    }
    result.back() = '}';
    cout << result << endl;
    return 0;
}

//436727