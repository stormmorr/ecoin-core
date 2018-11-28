#pragma once

#include "ecoinPage.g.h"

#ifdef ECOIN_ESTOCKS
#include "../../../eStocks/eStocks/eStocks/OpenGLESPage.xaml.h"
#endif

#ifdef ECOIN_INTERPLAY
#include "../../../Interplay/Interplay/Interplay/OpenGLESPage.xaml.h"
#endif

#ifdef ECOIN_GENERIC
#include "../../../Generic/Generic v4/Generic/Generic/DirectXPage.xaml.h"
using namespace Generic;
#endif

namespace ecoin
{
    public ref class ecoinPage sealed
    {
    public:
        virtual ~ecoinPage();

    internal:
        ecoinPage();

    private:
#ifdef ECOIN_MINER
		DirectXPage^ m_directXPage;
#endif
#ifndef ECOIN_MINER
#ifdef ECOIN_INTERPLAY
		Interplay::OpenGLESPage^ m_openglesPage;
#endif
#ifdef ECOIN_ESTOCKS
		eStocks::OpenGLESPage^ m_openglesPage;
#endif
		OpenGLES mOpenGLES;
#endif

		Windows::Foundation::IAsyncAction^ m_globalRandomWorker;

        void OnPageLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
		void OnStartecoin(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnClickOpenecoin(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
