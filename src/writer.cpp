#include "castle/writer.hpp"
#include "castle/lowlevel/common.hpp"
#include "castle/lowlevel/writeTag.hpp"
#include "castle/nbt.hpp"
#include <cstdint>
#include <vector>
namespace castle::nbt {

  constexpr std::string_view kDefaultWrapKey = "value";

  void writeRoot(const Nbt& node, std::vector<uint8_t>& data, WriteOptions options) {

    if (node.tag() == NbtTag::EndTag) {
      throw std::runtime_error("root tag can't be a endTag");
    }
    TagContext ctx = options.namelessRoot ? TagContext::NamelessRoot : TagContext::Named;
    if (node.tag() != NbtTag::CompoundTag && options.wrapNonCompoundRoot) {

      writeNbt(Nbt::makeCompound("",
                                 std::vector<Nbt>{node.name().empty()
                                                      ? node.renamed(std::string(kDefaultWrapKey))
                                                      : node}),
               data,
               ctx);
    } else {
      if (options.wrapNonCompoundRoot) {

        writeNbt(node, data, TagContext::NonCompoundRoot);
        // data.erase(data.begin(), data.begin() + 1);
      } else {

        writeNbt(node, data, ctx);
      }
    }
  }

  void NbtWriter::writer(const Nbt& tag, std::vector<std::uint8_t>& data, WriteOptions options) {
    writeRoot(tag, data, options);
  }
  std::vector<std::uint8_t> NbtWriter::writer(const Nbt& tag, WriteOptions options) {
    std::vector<std::uint8_t> output;

    writeRoot(tag, output, options);
    return output;
  }

} // namespace castle::nbt
