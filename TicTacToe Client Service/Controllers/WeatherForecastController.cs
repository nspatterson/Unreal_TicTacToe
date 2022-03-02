using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Amazon.GameLift;
using Amazon.GameLift.Model;
using Amazon;
using Amazon.Runtime;
using Amazon.Runtime.CredentialManagement;

namespace TicTacToe_Client_Service.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class WeatherForecastController : ControllerBase
    {
        private readonly ILogger<WeatherForecastController> _logger;

        public WeatherForecastController(ILogger<WeatherForecastController> logger)
        {
            _logger = logger;
        }

        [HttpGet]
        public async Task<IActionResult>Get()
        {
            var gameSession = await Oink.DescribeGameSesion(Oink.FleetId);
            var playerId = Guid.NewGuid().ToString();

            if (gameSession is null)
            {
#if LOCAL
                var createSession = await Oink.CreateGameSession(Oink.FleetId, playerId);
                gameSession = createSession.GameSession;
#else
                var session = await Oink.StartGameSessionPlacement(Oink.FleetId, playerId);

                if(session.Status == GameSessionPlacementState.FULFILLED)
                {
                    return Ok(session.PlacedPlayerSessions.FirstOrDefault(x => x.PlayerId == playerId).PlayerSessionId ?? "Not Found");
                }
                else
                {
                    return Ok("No Session Placement Found");
                }
#endif
            }
            var playerSessionResponse = await Oink.CreatePlayerSession(gameSession, playerId, "Name");

            if (playerSessionResponse is null)
            {
                return Ok("No Player Session Found");
            }

            _logger.Log(LogLevel.Information, $"Created player session Id: {playerSessionResponse.PlayerSession.PlayerSessionId}");
            return Ok(playerSessionResponse.PlayerSession);
        }
    }

    // This is just quick and dirty. probably should be placed in an interface/service/dependency injection but I don't feel like setting that up
    public static class Oink
    {
        private static AmazonGameLiftClient AwsClient;

        // TODO: Research where FleetId comes from. It's probably the ID of my other gamelift project
        public static string FleetId = "fleet-899cf40c-bf58-47fd-96c6-dbd6d150b7b8";

        static Oink()
        {
            // Initialize gamelift client in here
            var config = new AmazonGameLiftConfig();
            config.RegionEndpoint = RegionEndpoint.USEast1;
#if LOCAL
            //config.ServiceURL = "http://localhost:9080";
            //config.UseHttp = true;
#endif
            // TODO: Research how credientials are configured on EC2 instances and other cloud services
            AWSCredentials credentials;
            var store = new CredentialProfileStoreChain();
            store.TryGetAWSCredentials("player", out credentials);  // This might be specific to local

            // init game lift client to make requests
            AwsClient = new AmazonGameLiftClient(credentials, config);
        }

        public static async Task<CreateGameSessionResponse> CreateGameSession(string fleetId, string playerId)
        {
            var request = new CreateGameSessionRequest()
            {
                CreatorId = playerId,
                FleetId = fleetId,
                MaximumPlayerSessionCount = 2,
                Name = "Test Session",          // Figure out what this should be
            };

            var response = await AwsClient.CreateGameSessionAsync(request);

            return response;
        }

        public static async Task<CreatePlayerSessionResponse> CreatePlayerSession(GameSession gameSession, string playerId, string playerName)
        {
            var request = new CreatePlayerSessionRequest()
            {
                GameSessionId = gameSession.GameSessionId,
                PlayerData = playerName,
                PlayerId = playerId
            };

            var response = await AwsClient.CreatePlayerSessionAsync(request);

            return response;
        }

        public static async Task<GameSession> DescribeGameSesion(string fleetId)
        {
            var request = new DescribeGameSessionsRequest()
            {
                FleetId = fleetId,
                StatusFilter = "ACTIVE"
            };

            var response = await AwsClient.DescribeGameSessionsAsync(request);

            return response.GameSessions.FirstOrDefault(x => x.CurrentPlayerSessionCount < x.MaximumPlayerSessionCount);
        }

        public static async Task<GameSessionPlacement> StartGameSessionPlacement(string playerName, string playerId)
        {
            var request = new StartGameSessionPlacementRequest()
            {
                GameSessionQueueName = "Heh",  // Figure out what this is supposesd to be
                MaximumPlayerSessionCount = 2,
                PlacementId = Guid.NewGuid().ToString(),
                DesiredPlayerSessions = { new DesiredPlayerSession() { PlayerData = playerName, PlayerId = playerId } }
            };

            var response = await AwsClient.StartGameSessionPlacementAsync(request);

            return response.GameSessionPlacement;
        }

        public static async Task<GameSessionPlacement> DescribeGameSessionPlacement(string placementId)
        {
            var request = new DescribeGameSessionPlacementRequest()
            {
                PlacementId = placementId
            };

            var response = await AwsClient.DescribeGameSessionPlacementAsync(request);

            return response.GameSessionPlacement;
        }
    }
}
