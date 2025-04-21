#include <ROOT/RNTupleReader.hxx>

#include <ROOT/RNTupleUtil.hxx>
#include <ROOT/RNTupleView.hxx>
#include <TCanvas.h>
#include <TH1I.h>
#include <TROOT.h>
#include <TString.h>

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

using FVec = std::variant<std::vector<std::string>, std::vector<std::int32_t>,
                          std::vector<std::uint32_t>, std::vector<double>>;

std::vector<std::pair<std::string, std::string>>
GetFieldNamesAndTypes(const ROOT::REntry &entry) {
  std::vector<std::pair<std::string, std::string>> fields;

  for (const auto &val : entry) {
    fields.emplace_back(val.GetField().GetFieldName(),
                        val.GetField().GetTypeName());
  }

  return fields;
}

template <typename T>
inline void ProcessNumberField(std::ostream &out, const std::string &fieldName,
                               ROOT::RNTupleView<T> &view,
                               std::uint64_t entryId) {
  out << '"' << fieldName << "\":" << '"' << view(entryId) << '"';
}

inline void ProcessStringField(std::ostream &out, std::string_view fieldName,
                               ROOT::RNTupleView<std::string> &view,
                               std::uint64_t entryId) {
  out << '"' << fieldName << "\":\"" << view(entryId) << '"';
}

enum FieldTypes { String, Int32, Uint32, Double };

// constexpr char const *kNTupleFileName = "ntpl001_staff.root";
constexpr char const *kNTupleFileName = "B2HHH.ntuple.root";
// constexpr char const *kNTupleFileName = "ntpl002_vector.root";
constexpr char const *JSONName = "out.ndjson";

int main() {
  // auto reader = ROOT::RNTupleReader::Open("Staff", kNTupleFileName);
  // auto reader = RNTupleReader::Open("F", kNTupleFileName);
  auto reader = ROOT::RNTupleReader::Open("DecayTree", "B2HHH.ntuple.root");
  auto fields = GetFieldNamesAndTypes(reader->GetModel().GetDefaultEntry());
  std::ofstream file(JSONName);
  std::ostream &output = file;

  std::vector<std::pair<std::string, int>> fieldMap;
  std::vector<FVec> VFields;
  std::cout << reader->GetNEntries() << std::endl;

  for (const auto &[fieldName, fieldType] : fields) {
    if (fieldType == "std::string") {
      VFields.emplace_back(std::in_place_type<std::vector<std::string>>);
      auto &curVec = std::get<std::vector<std::string>>(VFields.back());
      curVec.reserve(reader->GetNEntries());
      fieldMap.emplace_back(fieldName, FieldTypes::String);
    } else if (fieldType == "std::int32_t") {
      VFields.emplace_back(std::in_place_type<std::vector<std::int32_t>>);
      auto &curVec = std::get<std::vector<std::int32_t>>(VFields.back());
      curVec.reserve(reader->GetNEntries());
      fieldMap.emplace_back(fieldName, FieldTypes::Int32);
    } else if (fieldType == "std::uint32_t") {
      VFields.emplace_back(std::in_place_type<std::vector<std::uint32_t>>);
      auto &curVec = std::get<std::vector<std::uint32_t>>(VFields.back());
      fieldMap.emplace_back(fieldName, FieldTypes::Uint32);
    } else if (fieldType == "double") {
      VFields.emplace_back(std::in_place_type<std::vector<double>>);
      auto &curVec = std::get<std::vector<double>>(VFields.back());
      fieldMap.emplace_back(fieldName, FieldTypes::Double);
    } else {
      std::cerr << "Found an unsupported fieldtype: " << fieldType << std::endl;
      exit(1);
    }
  }
  // bool first = true;
  // for (auto entryId : *reader) {
  //   first = true;
  //   output << '{';
  //   for (const auto &[fieldName, fieldType] : fieldMap) {
  //     if (!first) {
  //       output << ',';
  //     }
  //     first = false;
  //     switch (fieldType) {
  //     case FieldTypes::String: {
  //       auto view = reader->GetView<std::string>(fieldName);
  //       ProcessStringField(output, fieldName, view, entryId);
  //       break;
  //     }
  //     case FieldTypes::Int32: {
  //       auto view = reader->GetView<std::int32_t>(fieldName);
  //       ProcessNumberField(output, fieldName, view, entryId);
  //       break;
  //     }
  //     case FieldTypes::Uint32: {
  //       auto view = reader->GetView<std::uint32_t>(fieldName);
  //       ProcessNumberField(output, fieldName, view, entryId);
  //       break;
  //     }
  //     case FieldTypes::Double: {
  //       auto view = reader->GetView<double>(fieldName);
  //       ProcessNumberField(output, fieldName, view, entryId);
  //       break;
  //     }
  //     }
  //   }
  //   output << "}\n";
  // }
  file.close();

  return 0;
}
