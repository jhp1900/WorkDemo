#pragma once
//#include "resource.h"
#include <Windows.h>
#include "vlc/vlc.h"
#include <string>
#include "debug_tools.h"

using namespace std;

class TingFoxMediaPlayer
{
public:
	TingFoxMediaPlayer()
		: player_window_("")
		, tingfox_mediaplayer_instance_(nullptr)
		, tingfox_mediaplayer_(nullptr)
		, media_source_(nullptr)
	{}

	~TingFoxMediaPlayer(){}

	//初始化
	bool Initial()
	{
		if (!(tingfox_mediaplayer_instance_ = libvlc_new(0, NULL)))
			return false;
		else
			return true;

	}

	//设置窗口句柄
	bool SetPlayerWindows(void* player_window)
	{
		if (!player_window)
		{
			return false;
		} else
		{
			libvlc_media_player_set_hwnd(tingfox_mediaplayer_, player_window);
		}
		return true;
	}

	//设置播放资源
	bool SetMediaSource(string source_name)
	{
		if (empty(source_name))
		{
			return false;
		} else
		{
			media_source_ = libvlc_media_new_location(tingfox_mediaplayer_instance_, source_name.c_str());
			tingfox_mediaplayer_ = libvlc_media_player_new_from_media(media_source_);
			return true;
		}
	}

	//播放
	void StartPlay()
	{
		if (!tingfox_mediaplayer_)
			return;
		debugPrintf("tingfox_mediaplayer is null");
		libvlc_media_player_play(tingfox_mediaplayer_);
	}


	void Destory()
	{
		libvlc_media_release(media_source_);
		libvlc_media_player_release(tingfox_mediaplayer_);
		libvlc_release(tingfox_mediaplayer_instance_);
	}

private:
	string player_window_;

	libvlc_instance_t* tingfox_mediaplayer_instance_;
	libvlc_media_player_t* tingfox_mediaplayer_;
	libvlc_media_t* media_source_;
};

//TingFoxMediaPlayer::TingFoxMediaPlayer()
//	: player_window_("")
//	, tingfox_mediaplayer_instance_(nullptr)
//	, tingfox_mediaplayer_(nullptr)
//	, media_source_(nullptr)
//{}
//
//bool TingFoxMediaPlayer::Initial()
//{
//	if (!(tingfox_mediaplayer_instance_ = libvlc_new(0, NULL)))
//		return false;
//	else
//		return true;
//
//}
//bool TingFoxMediaPlayer::SetMediaSource(string source_name)
//{
//	if (empty(source_name))
//	{
//		return false;
//	}
//	else
//	{
//		media_source_ = libvlc_media_new_location(tingfox_mediaplayer_instance_, source_name.c_str());
//		tingfox_mediaplayer_ = libvlc_media_player_new_from_media(media_source_);
//		return true;
//	}
//}
//
//bool TingFoxMediaPlayer::SetPlayerWindows(void* player_window)
//{
//	if (!player_window)
//	{
//		return false;
//	}
//	else
//	{
//		libvlc_media_player_set_hwnd(tingfox_mediaplayer_, player_window);
//	}
//	return true;
//}
//
//void TingFoxMediaPlayer::StartPlay()
//{
//	if (!tingfox_mediaplayer_)
//		return;
//	debugPrintf("tingfox_mediaplayer is null");
//	libvlc_media_player_play(tingfox_mediaplayer_);
//}
//
//void TingFoxMediaPlayer::Destory()
//{
//	libvlc_media_release(media_source_);
//	libvlc_media_player_release(tingfox_mediaplayer_);
//	libvlc_release(tingfox_mediaplayer_instance_);
//}

//TingFoxMediaPlayer::~TingFoxMediaPlayer()
//{}
//
//int main(int argc, char* argv[])
//{
//	TingFoxMediaPlayer TFMPlayer;
//	TFMPlayer.Initial();
//	TFMPlayer.SetMediaSource("rtsp://192.0.0.122/11");
//	TFMPlayer.StartPlay();
//	Sleep(1000);
//	TFMPlayer.Destory();
//	return 0;
//}