// Copyleft Paul M. Bendixen 2017
// This file is released under GNU LGPL
#ifndef REGISTER_H
#define REGISTER_H

#include <cstdint>
#include <cstddef>

template < std::size_t address, typename ValueType, ValueType mask, std::uint8_t offset, typename mutability >
struct Register
{
	static void write( ValueType value )
	{
		mutability::write( address, mask, offset, value );
	}
	static ValueType read()
	{
		mutability::read( address, mask, offset );
	}
};


template < typename ValueType >
struct WriteOnly
{
	static void write( std::size_t address, ValueType mask, std::uint8_t offset, ValueType value )
	{
		*reinterpret_cast< volatile ValueType* >( address ) = ( value & mask ) << offset;
	}
};

template < typename ValueType >
struct ReadOnly
{
	static ValueType read( std::size_t address, ValueType mask, std::uint8_t offset )
	{
		return ( *reinterpret_cast< const volatile ValueType* >( address ) & mask ) >> offset;
	}
};

template < typename ValueType >
struct ReadWrite : ReadOnly< ValueType >
{
	static void write( std::size_t address, ValueType mask, std::uint8_t offset, ValueType value )
	{
		*reinterpret_cast< volatile ValueType* >( address ) = 
			( *reinterpret_cast< const volatile ValueType* >( address ) & ~( mask ) ) |
			( ( value & mask ) << offset );
	}
};

using RO8 = ReadOnly< std::uint8_t >;
using RO16 = ReadOnly< std::uint16_t >;
using RW8 = ReadWrite< std::uint8_t >;
using RW16 = ReadWrite< std::uint16_t >;
using WO8 = WriteOnly< std::uint8_t >;
using WO16 = WriteOnly< std::uint16_t >;

#endif //REGISTER_H
