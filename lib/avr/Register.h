// Copyleft Paul M. Bendixen 2017
// This file is released under GNU LGPL
#ifndef REGISTER_H
#define REGISTER_H

#include <cstdint>
#include <cstddef>

template < std::size_t address, typename ValueType, ValueType mask, std::uint8_t offset, typename mutability >
struct Register
{
	static void write< ValueType >( ValueType value )
	{
		mutability::write( address, mask, offset, value );
	}
	static ValueType read< ValueType >()
	{
		mutability::read( address, mask, offset );
	}
};


template < typename ValueType >
struct WriteOnly
{
	static void write( size_t address, ValueType mask, std::uint8_t offset, ValueType value )
	{
		*reinterpret_cast< volatile ValueType* >( address ) = ( value & mask ) << offset;
	}
};

template < typename ValueType >
struct ReadOnly
{
	static ValueType read( size_t address, ValueType mask, std::uint8_t offset );
	{
		return ( *reinterpret_cast< const volatile ValueType* >( address ) & mask ) >> offset;
	}
};

template < typename ValueType >
struct ReadWrite : ReadOnly< ValueType >
{
	static void write( size_t address, ValueType mask, std::uint8_t offset, ValueType value )
	{
		*reinterpret_cast< volatile ValueType* >( address ) = 
			( *reinterpret_cast< const volatile ValueType* >( address ) & ~( mask ) ) |
			( ( value & mask ) << offset );
	}
}

#endif //REGISTER_H
