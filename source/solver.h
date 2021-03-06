/**
 * @file solver.h
 * @brief Header file for class \ref solver
 */

#pragma once

#include "eddy.h"

class domain;

using namespace std;

////////////////////////////////////////////////////////////////////////////////

/** Class implementing solver object
 *
 *  @author David O. Lignell
 */

class solver {


    //////////////////// DATA MEMBERS //////////////////////

    public:

        domain         *domn;          ///< pointer to domain object

        eddy           *ed3;           ///< pointer to eddy object for thirds
        domain         *eddl3;         ///< pointer to eddy line object

        double         time;           ///< odt time (during sampling)
        double         t0;             ///< time of last eddy event; diffusion left off here.
        double         dtSmean;        ///< initial mean eddy sample time
        double         dtCUmax;        ///< max time before catch up diff/eddy

        bool           LeddyAccepted;  ///< flag for accepted eddy
        int            iEtrials;       ///< number of eddy trials

        double         PaSum;          ///< sum of Pa of eddies
        int            nPaSum;         ///< number going into PaSum
        int            neddies;        ///< number of eddies accepted
        double         PaSumC;         ///< sum of Pa of eddies
        int            nPaSumC;        ///< number going into PaSum

    //////////////////// MEMBER FUNCTIONS /////////////////

    public:

        virtual void calculateSolution();

    private:

        bool   sampleEddyAndImplementIfAccepted();
        void   computeDtSmean();
        void   computeDtCUmax();
        double sampleDt();
        void   diffusionCatchUpIfNeeded(bool Ldoit=false);
        void   raiseDtSmean();
        void   lowerDtSmean();
        bool   testLES_elapsedTime(const double time, const double tauEddy);
        bool   testLES_fracDomain( const double eSize);
        bool   testLES_integralLength(const double time, const double eSize);
        bool   testLES_thirds();


    //////////////////// CONSTRUCTOR FUNCTIONS /////////////////

    public:

        solver(){ ed3=0; eddl3=0; }
        virtual void init(domain *p_domn);
        virtual ~solver();

};


////////////////////////////////////////////////////////////////////////////////


