/*
 * Copyright (C) 2005 - 2011, Roni Zaharia
 */

/// \page CPPTestApplications C++ Test Applications
/// The C++ test applications can be downloaded from www.roniza.com/downloads
/// \section CPPMWLSCU C++ Modality Worklist Query SCU Example
/// C++ Modality Worklist Query SCU Example
/// This example shows how to perform a Modality Worklist Query (MWL)

#include <stdio.h>
#include <time.h>
#include <string>
#include <list>
#include <atlbase.h>
#include "atlconv.h"

/// Dll import from the current directory
#import "../rzdcx.tlb" rename_namespace ("rzdcxLib")
using namespace rzdcxLib;
using namespace std;

/// \example CPPQueryRetrieveSCUExample.cpp
/// This example shows how to send a DICOM Query to the target AE
/// on patients and how to retrieve the data (Move)
/// 1) "Query" or "Move"
/// 2) LocalAE
/// 3) TargetAE
/// 4) Host
/// 5) Port
/// 6) AE to move to (only with Move) 
/// 7) Patient ID to move (only for Move)

#define AddTagS( a )  { el->Init( (int)a ); p->insertElement(el); }
#define AddTagS2( a )  { el->Init( (int)a ); p2->insertElement(el); }
#define AddTag( a )  { el->Init( (int)a ); o->insertElement(el); }
#define AddTagV( a,v )  { el->Init( (int)a ); el->Value=v; o->insertElement(el); }


void Query()
{
	IDCXREQPtr req(__uuidof(DCXREQ));
	IDCXOBJPtr o(__uuidof(DCXOBJ));
	IDCXELMPtr el( __uuidof(DCXELM ) );
	IDCXOBJIteratorPtr it(__uuidof( DCXOBJIterator ) );
	try
	{

		AddTag( StudyDate );
		AddTag( AccessionNumber );
		AddTag( ReferringPhysiciansName );

		{
			IDCXOBJPtr p(__uuidof(DCXOBJ));
			IDCXOBJIteratorPtr it1(__uuidof( DCXOBJIterator ) );
			AddTagS( ReferencedSOPClassUID );
			AddTagS( ReferencedSOPInstanceUID );
			it1->Insert(p);
			el->Init( ReferencedStudySequence );
			el->PutValue( &_variant_t( (IUnknown*)it1, false ) );
			o->insertElement( el );
		}
		{
			IDCXOBJPtr p(__uuidof(DCXOBJ));
			IDCXOBJIteratorPtr it1(__uuidof( DCXOBJIterator ) );
			AddTagS( ReferencedSOPClassUID );
			AddTagS( ReferencedSOPInstanceUID );
			it1->Insert(p);
			el->Init( ReferencedPatientSequence );
			el->PutValue( &_variant_t( (IUnknown*)it1, false ) );
			o->insertElement( el );
		}

		AddTagV( PatientsName, "Patient^Name" );
		AddTagV( patientID, "*");
		AddTag( PatientsBirthDate );
		AddTag( PatientsSex );
		AddTag( OtherPatientIDs );
		AddTag( PatientsSize );
		AddTag( PatientsWeight );
		AddTag( AdditionalPatientHistory );
		AddTag( PatientComments );
		AddTag( studyInstanceUID );
		AddTag( RequestingPhysician );

		{
			IDCXOBJPtr p(__uuidof(DCXOBJ));
			IDCXOBJIteratorPtr it1(__uuidof( DCXOBJIterator ) );
			AddTagS( CodeValue );
			AddTagS( CodingSchemeDesignator );
			AddTagS( CodeMeaning );
			it1->Insert(p);
			el->Init( RequestedProcedureCodeSequence );
			el->PutValue( &_variant_t( (IUnknown*)it1, false ) );
			o->insertElement( el );
		}

		AddTag( AdmissionID );
		AddTag( CurrentPatientLocation );
		AddTag( PatientState );


		{
			IDCXOBJPtr p(__uuidof(DCXOBJ));
			IDCXOBJIteratorPtr it1(__uuidof( DCXOBJIterator ) );

			el->Init( Modality );
			//el->Value = "";
			p->insertElement(el);

			AddTagS( ScheduledStationAETitle );
			AddTagS( ScheduledProcedureStepStartDate );
			AddTagS( ScheduledProcedureStepStartTime );
			AddTagS( ScheduledPerformingPhysiciansName );
			AddTagS( ScheduledProcedureStepDescription );

			{
				IDCXOBJPtr p2(__uuidof(DCXOBJ));
				IDCXOBJIteratorPtr it2(__uuidof( DCXOBJIterator ) );
				AddTagS2( CodeValue );
				AddTagS2( CodingSchemeDesignator );
				AddTagS2( CodeMeaning );
				it2->Insert(p2);
				el->Init( ScheduledProtocolCodeSequence );
				el->PutValue( &_variant_t( (IUnknown*)it2, false ) );
				p->insertElement( el );
			}

			AddTagS( ScheduledProcedureStepID );
			AddTagS( ScheduledStationName );
			AddTagS( ScheduledProcedureStepLocation );
			AddTagS( CommentsOnTheScheduledProcedureStep );
			AddTagV( ScheduledProcedureStepStartDate, "20100427-20100430");

			it1->Insert(p);
			el->Init( ScheduledProcedureStepSequence );
			el->PutValue( &_variant_t( (IUnknown*)it1, false ) );
			o->insertElement( el );
		}


		AddTag( RequestedProcedureID );
		AddTag( ConfidentialityConstraintOnPatientDataDescription );


		it = req->Query( "CMD", "MWLSCP1", "localhost", 6104, "1.2.840.10008.5.1.4.31" /* The MWL SOP Class UID */, o );


		// Iterate over the query results
		//for (; !it.AtEnd(); it.Next())
		for ( ; !it->AtEnd(); it->Next() )
		{
			IDCXOBJPtr r(__uuidof(DCXOBJ));
			r = it->Get();

			IDCXELMPtr e(0);
			try {
				e = r->getElementByTag( RequestedProcedureID );
				if (e && e->Value)
					_bstr_t s1 = e->Value.bstrVal;
			} catch (_com_error& e) {
				_bstr_t s = e.Description();
			}

			try {
				e = r->getElementByTag( RequestedProcedureDescription );
				if (e && e->Value)
					_bstr_t s2 = e->Value.bstrVal;
			} catch (_com_error& e) {
				_bstr_t s = e.Description();
			}

			try {
				e = r->getElementByTag( patientID );
				if (e && e->Value)
					_bstr_t s3 = e->Value.bstrVal;
			} catch (_com_error& e) {
				_bstr_t s = e.Description();
			}

			try {
				e = r->getElementByTag( PatientsBirthDate );
				if (e && e->Value)
					_bstr_t s4 = e->Value.bstrVal;
			} 
			catch (_com_error& e) {
				_bstr_t s = e.Description();
			}

		}
	}
	catch (...)
	{
	}
}

int main(int argc, char* argv[])
{
	HRESULT hr = ::CoInitialize(NULL); 

	if (SUCCEEDED(hr)){
		Query();
	}

	::CoUninitialize();
	return 0;      
}

