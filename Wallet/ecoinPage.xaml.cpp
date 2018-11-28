#include "pch.h"
#include "ecoinPage.xaml.h"

using namespace Platform;
using namespace Concurrency;
using namespace Windows::Foundation;

using namespace ecoin;

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

using namespace Windows::Storage::Streams;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Media::Transcoding;
using namespace Windows::Media::MediaProperties;
using namespace Windows::UI::Xaml::Shapes;
using namespace Windows::UI::Xaml;
using namespace Windows::Storage::Provider;

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Store;
using namespace Windows::Globalization;

using namespace Windows::System::Threading;
using namespace Windows::System;

ecoinPage::ecoinPage()
{
    InitializeComponent();

    Windows::UI::Core::CoreWindow^ window = Windows::UI::Xaml::Window::Current->CoreWindow;

    //window->VisibilityChanged +=
    //    ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::VisibilityChangedEventArgs^>(this, &ecoinPage::OnVisibilityChanged);

    //this->Loaded +=
    //    ref new Windows::UI::Xaml::RoutedEventHandler(this, &ecoinPage::OnPageLoaded);

#if 0
	// Create a task that will be run on a background thread.
	auto globalRandomHandler = ref new WorkItemHandler([this](IAsyncAction ^ action)
		{
		// Calculate the updated frame and render once per vertical blanking interval.
		while(g_Begin)
			{
			int f_Int = rand();
			printf("%i", f_Int);
			}
		});

	// Run task on a dedicated high priority background thread.
	m_globalRandomWorker = ThreadPool::RunAsync(globalRandomHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
#endif
}

ecoinPage::~ecoinPage()
{
}

void ecoinPage::OnPageLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//Usernametextblock->Text = StrUsingStr(g_TradeManager->m_Username);
	//Tickertextblock->Text = StrUsingStr(g_TradeManager->m_MessageOTD);
}

void ecoinPage::OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args)
{
}

void ag_Randomise(void)
{
	/*double f_Balance = g_Wallet->GetBalance(false);
	int f_NomSpins = f_Balance / 50 + 5.0f;

	srand(f_Balance);

	for(int f_Spin = 0; f_Spin < f_NomSpins; f_Spin++)
		{
		int f_Finger = rand() % 5000;
		printf("%i", f_Finger);
		}*/
}

void ecoin::ecoinPage::OnStartecoin(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	g_Wallet->m_Start = true;
	g_Wallet->Open();

	ag_Randomise();

	g_Begin = false;

	if(!g_Wallet->m_Repeat)
		{
#ifdef ECOIN_MINER
		if(m_directXPage == nullptr)
			{
			m_directXPage = ref new DirectXPage();

			g_DirectXPage = m_directXPage;
			}

		// Place the page in the current window and ensure that it is active.
		Window::Current->Content = m_directXPage;
		Window::Current->Activate();
#endif
#ifdef ECOIN_INTERPLAY
		if(m_openglesPage == nullptr)
			{
			m_openglesPage = ref new OpenGLESPage(&mOpenGLES);
			}

		// Place the page in the current window and ensure that it is active.
		Windows::UI::Xaml::Window::Current->Content = m_openglesPage;
		Windows::UI::Xaml::Window::Current->Activate();
#endif
#ifdef ECOIN_ESTOCKS
		if(m_openglesPage == nullptr)
			{
			m_openglesPage = ref new OpenGLESPage(&mOpenGLES);
			}

		// Place the page in the current window and ensure that it is active.
		Windows::UI::Xaml::Window::Current->Content = m_openglesPage;
		Windows::UI::Xaml::Window::Current->Activate();
#endif

#ifndef ECOIN_MINER
#ifndef ECOIN_INTERPLAY
		throw;
#endif
#endif
		}
}


void ecoin::ecoinPage::OnClickOpenecoin(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	g_Wallet->m_Start = false;
	g_Wallet->Open();

	ag_Randomise();

	g_Begin = false;

	if(!g_Wallet->m_Repeat)
		{
#ifdef ECOIN_MINER
		if(m_directXPage == nullptr)
			{
			m_directXPage = ref new DirectXPage();

			g_DirectXPage = m_directXPage;
			}

		// Place the page in the current window and ensure that it is active.
		Window::Current->Content = m_directXPage;
		Window::Current->Activate();
#endif
#ifndef ECOIN_MINER
		if(m_openglesPage == nullptr)
			{
#ifdef ECOIN_ESTOCKS
			m_openglesPage = ref new eStocks::OpenGLESPage(&mOpenGLES);
#endif
#ifdef ECOIN_INTERPLAY
			m_openglesPage = ref new Interplay::OpenGLESPage(&mOpenGLES);
#endif
			}

		// Place the page in the current window and ensure that it is active.
		Windows::UI::Xaml::Window::Current->Content = m_openglesPage;
		Windows::UI::Xaml::Window::Current->Activate();
#endif

#ifndef ECOIN_MINER
#ifndef ECOIN_INTERPLAY
#ifndef ECOIN_ESTOCKS
		throw;
#endif
#endif
#endif
		}
}
