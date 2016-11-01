#pragma once
#include "stdafx.h"
#include "vlc/vlc.h"
#include <string>

using namespace std;

class TingFoxMediaPlayer
{
public:
	TingFoxMediaPlayer();
	~TingFoxMediaPlayer();
	//��ʼ��
	bool Initial();
	//���ô��ھ��
	bool SetPlayerWindows(void* player_window);
	//���ò�����Դ
	bool SetMediaSource(string source_name);
	//����
	void StartPlay();
	void Destory();

private:
	string player_window_;

	libvlc_instance_t* tingfox_mediaplayer_instance_;
	libvlc_media_player_t* tingfox_mediaplayer_;
	libvlc_media_t* media_source_;
};

TingFoxMediaPlayer::TingFoxMediaPlayer()
	: player_window_("")
	, tingfox_mediaplayer_instance_(nullptr)
	, tingfox_mediaplayer_(nullptr)
	, media_source_(nullptr)
{}

bool TingFoxMediaPlayer::Initial()
{
	if (!(tingfox_mediaplayer_instance_ = libvlc_new(0, NULL)))
		return false;
	else
		return true;

}
bool TingFoxMediaPlayer::SetMediaSource(string source_name)
{
	if (source_name.empty())
	{
		return false;
	}
	else
	{
		media_source_ = libvlc_media_new_location(tingfox_mediaplayer_instance_, source_name.c_str());
		tingfox_mediaplayer_ = libvlc_media_player_new_from_media(media_source_);
		return true;
	}
}

bool TingFoxMediaPlayer::SetPlayerWindows(void* player_window)
{
	if (!player_window)
	{
		return false;
	}
	else
	{
		libvlc_media_player_set_hwnd(tingfox_mediaplayer_, player_window);
	}
	return true;
}

void TingFoxMediaPlayer::StartPlay()
{
	if (!tingfox_mediaplayer_)
		return;
	libvlc_media_player_play(tingfox_mediaplayer_);
}

void TingFoxMediaPlayer::Destory()
{
	libvlc_media_release(media_source_);
	libvlc_media_player_release(tingfox_mediaplayer_);
	libvlc_release(tingfox_mediaplayer_instance_);
}

TingFoxMediaPlayer::~TingFoxMediaPlayer()
{}

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