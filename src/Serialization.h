#pragma once



namespace Serialization
{
	static constexpr std::uint32_t SerializationVersion = 1;
	static constexpr std::uint32_t ID = 'SPNG';
	static constexpr std::uint32_t SerializationType = 'SPNV';

	void SaveCallback(SKSE::SerializationInterface* a_skse);
	void LoadCallback(SKSE::SerializationInterface* a_skse);
	void RevertCallback([[maybe_unused]] SKSE::SerializationInterface* a_skse);
}