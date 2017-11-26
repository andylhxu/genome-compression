#include "ModelOrder0C.h"

ModelOrder0C::ModelOrder0C()
{
	// initialize probabilities with 1
	mTotal = 257; // 256 + escape symbol for termination
	for( unsigned int i=0; i<257; i++ )
		mCumCount[i] = 1;
}

void ModelOrder0C::Encode()
{

	while( !mSource->eof() )
	{
		unsigned char symbol;

		// read symbol
		mSource->read( reinterpret_cast<char*>(&symbol), sizeof( symbol ) );

		if( !mSource->eof() )
		{
			// cumulate frequencies
			unsigned int low_count = 0;
			unsigned char j; //added, corrected for new for loop variable scoping convention
			for( j=0; j<symbol; j++ )
				low_count += mCumCount[j];

			// encode symbol
			mAC.Encode( low_count, low_count + mCumCount[j], mTotal );

			// update model
			mCumCount[ symbol ]++;
			mTotal++;
		}
	}

	// write escape symbol for termination
	mAC.Encode( mTotal-1, mTotal, mTotal );
}

void ModelOrder0C::Decode()
{
	unsigned int symbol;

	do
	{
		unsigned int value;

		// read value
		value = mAC.DecodeTarget( mTotal );

		unsigned int low_count = 0;

		// determine symbol
		for( symbol=0; low_count + mCumCount[symbol] <= value; symbol++ )
			low_count += mCumCount[symbol];

		// write symbol
		if( symbol < 256 )
			mTarget->write( reinterpret_cast<char*>(&symbol), sizeof( char ) );

		// adapt decoder
		mAC.Decode( low_count, low_count + mCumCount[ symbol ] );

		// update model
		mCumCount[ symbol ]++;
		mTotal++;
	}
	while( symbol != 256 ); // until termination symbol read
}
