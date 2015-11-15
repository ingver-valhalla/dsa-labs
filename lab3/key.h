// key.h

#ifndef KEY_H
#define KEY_H

class Key
{
public:
	Key() : m(0) {}
	Key( int m ) : m(m) {}
	Key( const Key& k ) : m(k.m) {}

	int val()
	{
		return m;
	}

	const Key& operator=( int val )
	{
		m = val;
		++assigns;
		return *this;
	}
	
	bool operator==( const Key& k ) const
	{
		++comps;
		return m == k.m;
	}

	bool operator!=( const Key& k ) const
	{
		++comps;
		return m != k.m;
	}

	bool operator<( const Key& k ) const
	{
		++comps;
		return m < k.m;
	}

	bool operator>( const Key& k ) const
	{
		return k < *this ;
	}

	static int comparisons()
	{
		return comps;
	}

	static void reset_comps()
	{
		comps = 0;
	}

	static int assignments()
	{
		return assigns;		
	}

	static void reset_assigns()
	{
		assigns = 0;
	}

	/*
	Key& operator+( Key& k )
	{
		return Key( m + k.m );
	}

	Key& operator-( Key& k )
	{
		return Key( m - k.m );
	}

	Key& operator*( Key& k )
	{
		return Key( m * k.m );		
	}

	Key& operator/( Key& k )
	{
		return Key( m / k.m );
	}
	*/
private:
	int m;
	static int assigns;
	static int comps;
};

#endif // KEY_H
